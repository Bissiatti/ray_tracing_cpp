#include <iostream>
#include <cmath>
#include <random>
using namespace std;
#include "linalg.h"
#include "ray.h"
#include "sphere.h"
#include "cam.h"

#include "film.h"
#include "light.h"
#include <vector>

std::default_random_engine generator; // Cria um gerador de números aleatórios

vec3 Radiance(Light light, vec3 pointLocal, vec3 normal, vec3 lightDir, vector<Shape *> &scene, string Type, vec3 colorObj)
{
    vec3 color;
    Ray rayLight = Ray(light.position, lightDir);
    // check if the light is visible from the point
    float minT = INFINITY;
    for (int j = 0; j < scene.size(); j++)
    {
        float t = scene[j]->rayIntersection(rayLight.origin, rayLight.direction, j);
        if (t < minT)
        {
            minT = t;
          
        }
    }
    if (minT == INFINITY)
    {
        return vec3(0, 0, 0);
    }
    vec3 point = rayLight.origin + rayLight.direction * minT;
    if ((point - pointLocal).length() < 0.00001)
    {
        if (Type == "Plane")
        {
            vec3 color1 = colorObj * light.color * std::max(0.0f, normal.dot(lightDir));
            vec3 color2 = colorObj * light.color * std::max(0.0f, normal.dot(lightDir * -1));
            color = color1.max(color2);
        }
        if (Type == "Sphere")
        {
            vec3 h = (lightDir - pointLocal).unit();
            color = colorObj * light.color * std::max(0.0f, normal.dot(h));
        }
    }
    return color;
}

vec3 Radiance(RectangleLight light, vec3 pointLocal, vec3 normal, vec3 lightDir, vec3 lightPos, vector<Shape *> &scene, string Type, vec3 colorObj)
{
    vec3 color;
    Ray rayLight = Ray(lightPos, lightDir);
    // check if the light is visible from the point
    float minT = INFINITY;
    for (int j = 0; j < scene.size(); j++)
    {
        float t = scene[j]->rayIntersection(rayLight.origin, rayLight.direction, j);
        if (t < minT)
        {
            minT = t;
           
        }
    }
    if (minT == INFINITY)
    {
        return vec3(0, 0, 0);
    }
    vec3 point = rayLight.origin + rayLight.direction * minT;
    if ((point - pointLocal).length() < 0.00001)
    {
        if (Type == "Plane")
        {
            vec3 color1 = colorObj * light.color * std::max(0.0f, normal.dot(lightDir));
            vec3 color2 = colorObj * light.color * std::max(0.0f, normal.dot(lightDir * (-1)));
            color = color1.max(color2);
        }
        if (Type == "Sphere")
        {
            vec3 h = (lightDir - pointLocal).unit();
            color = colorObj * light.color * std::max(0.0f, normal.dot(h));
        }
    }
    return color;
}


vec3 Radiance(RectangleLight light, vec3 pointLocal, vec3 normal, vec3 lightDir, vec3 lightPos, vector<Shape *> &scene, string Type, vec3 colorObj, int depth)
{
    vec3 color;
    Ray rayLight = Ray(lightPos, lightDir);
    // check if the light is visible from the point
    float minT = INFINITY;
    float i = 0;
    for (int j = 0; j < scene.size(); j++)
    {
        float t = scene[j]->rayIntersection(rayLight.origin, rayLight.direction, j);
        if (t < minT)
        {
            minT = t;
            i = j;
            
        }
    }

    vec3 point = rayLight.origin + rayLight.direction * minT;
    if ((point - pointLocal).length() < 0.00001)
    {

        if (Type == "ReflectiveSphere")
        {
            if (minT == INFINITY)
            {
                return vec3(0, 0, 0);
            }
            if (depth <= 0)
            {
                return vec3(0, 0, 0);
            }
            vec3 h = (lightDir - pointLocal).unit();
            color = colorObj * light.color * std::max(0.0f, normal.dot(h));
            vec3 direction = random_on_hemisphere(normal);
            return Radiance(light, pointLocal, normal, direction, lightPos, scene, Type, colorObj, depth) * 0.5 + color * 0.05;
        }
    }
}

vec3 RadianceReflect(RectangleLight light, vec3 pointLocal, vec3 normal, vec3 reflectDir, vec3 lightPos, vector<Shape *> &scene, string Type, vec3 colorObj, vec3 att, int depth){
    vec3 color;
    Ray rayReflect = Ray(pointLocal, reflectDir);
    float minT = INFINITY;
    float i = 0;
    for (int j = 0; j < scene.size(); j++)
    {
        float t = scene[j]->rayIntersection(rayReflect.origin, rayReflect.direction, j);
        if (t < minT)
        {
            minT = t;
            i = j;
        }
    }

    if (minT == INFINITY){
        return vec3(0,0,0);
    }

    if (depth<1){
        return vec3(1,1,1);
    }

    if (scene[i]->getName() == "MetalSphere"){
        return RadianceReflect(light,  pointLocal,  normal,  reflectDir,  lightPos,scene, Type,  colorObj,  att, depth-1) *att;
    }
    else{
        vec3 pointLocal = rayReflect.origin + rayReflect.direction * minT;
        vec3 lightDir = (pointLocal - lightPos).unit();
        if (scene[i]->getName() == "ReflectiveSphere")
        {

            vec3 normal = (pointLocal - scene[i]->getCenter()).unit();
            return att*Radiance(light, pointLocal, normal, lightDir, lightPos, scene, "ReflectiveSphere", scene[i]->getColor(), 25);
        }
        if (scene[i]->getName() == "Sphere")
        {
            vec3 normal = (pointLocal - scene[i]->getCenter()).unit();
            return att*Radiance(light, pointLocal, normal, lightDir, lightPos, scene, "Sphere", scene[i]->getColor());
        }
        if (scene[i]->getName() == "Plane")
        {
            vec3 normal = scene[i]->getNormal();
            return att*Radiance(light, pointLocal, normal, lightDir, lightPos, scene, "Plane", scene[i]->getColor());
        }
    }
}


vector<vec3> Render(vector<Shape *> &scene, Camera cam, Film *film, RectangleLight light, int nSamples, int nSamplesLight, string typeSamples)
{
    int maxY = film->resolution * cam.ratio;
    film->pixels = vector<vec3>(maxY * film->resolution);
   
    int m1 = sqrt(nSamplesLight);
    int m2 = sqrt(nSamplesLight) + 1;
    int m3 = m1 * m2;
   
    float dx = 1.0 / m1;
    float dy = 1.0 / m2;
   
    for (int y = 0; y < maxY; y++)
    {
        for (int x = 0; x < film->resolution; x++)
        {
            vec3 color = vec3(0, 0, 0);
            for (int sample = 0; sample < nSamples * nSamplesLight; sample++)
            {

                float lightX;
                float lightY;

                if (typeSamples == "stratified")
                {
                    lightX = ((sample % m3) % m1) * dx + (double)rand() / RAND_MAX * dx;
                    lightY = ((sample % m3) / m1) * dy + (double)rand() / RAND_MAX * dy;
                }
                else if (typeSamples == "standard")
                {
                    lightX = ((sample % m3) % m1) * dx + dx / 2;
                    lightY = ((sample % m3) / m1) * dy + dy / 2;
                }
                else if (typeSamples == "normal")
                {
                    lightX = normal_distribution<double>(((sample % m3) % m1) * dx + dx / 2, dx / 2)(generator);
                    lightY = normal_distribution<double>(((sample % m3) / m1) * dy + dy / 2, dy / 2)(generator);
                }
                else if (typeSamples == "normalCenter")
                {
                    lightX = normal_distribution<double>(0.5 + 0.15)(generator);
                    lightY = normal_distribution<double>(0.5 + 0.15)(generator);
                }
                else
                {
                    lightX = (double)rand() / RAND_MAX;
                    lightY = (double)rand() / RAND_MAX;
                }

                float nx = (float)(x + (double)rand() / RAND_MAX) / film->resolution;
                float ny = (float)(y + (double)rand() / RAND_MAX) / film->resolution;
              
                Ray r = cam.generateRay(nx, ny);
                float minT = INFINITY;
                int i = 0;
                for (int j = 0; j < scene.size(); j++)
                {
                    float t = scene[j]->rayIntersection(r.origin, r.direction, j);
                    if (t < minT)
                    {
                        minT = t;
                        i = j;
                    }
                }
                if (minT < INFINITY)
                {
                    vec3 sampleColor = vec3(0, 0, 0);
                    vec3 pointLocal = r.origin + r.direction * minT;
                    vec3 lightDir = (pointLocal - (light.position + light.width * lightX + light.height * lightY)).unit();

                    vec3 lightPos = light.position + light.width * lightX + light.height * lightY;
                     
                    if (scene[i]->getName() == "ReflectiveSphere")
                    {
                        

                        vec3 normal = (pointLocal - scene[i]->getCenter()).unit();
                        sampleColor = Radiance(light, pointLocal, normal, lightDir, lightPos, scene, "ReflectiveSphere", scene[i]->getColor(), 25);
                    }
                    if (scene[i]->getName() == "MetalSphere")
                    {
                      
                        vec3 normal = (pointLocal - scene[i]->getCenter()).unit();
                        vec3 ref = reflect(r.direction, normal);
                        sampleColor = RadianceReflect(light,pointLocal,normal,ref,lightPos,scene,"MetalSphere",scene[i]->getColor(),scene[i]->getAtt(),25);
                    }

                    if (scene[i]->getName() == "Sphere")
                    {
                        vec3 normal = (pointLocal - scene[i]->getCenter()).unit();
                        sampleColor = Radiance(light, pointLocal, normal, lightDir, lightPos, scene, "Sphere", scene[i]->getColor());
                    }
                    if (scene[i]->getName() == "Plane")
                    {
                        vec3 normal = scene[i]->getNormal();
                        sampleColor = Radiance(light, pointLocal, normal, lightDir, lightPos, scene, "Plane", scene[i]->getColor());
                    }
                    color += sampleColor;
                }
                if (minT == INFINITY)
                {
                    if (light.rayIntersectionArea(r.origin, r.direction))
                    {
                        color += light.color * nSamples;
                    }
                }
            }
            color /= nSamples;
            film->setPixel(x, y, color);
        }
    }
    return film->pixels;
}

vector<vec3> Render(vector<Shape *> &scene, Camera cam, Film *film, Light light, int nSamples)
{
    int maxY = film->resolution * cam.ratio;
    film->pixels = vector<vec3>(maxY * film->resolution);
    
    for (int y = 0; y < maxY; y++)
    {
        for (int x = 0; x < film->resolution; x++)
        {
            vec3 color = vec3(0, 0, 0);
            for (int sample = 0; sample < nSamples; sample++)
            {
                float nx = (float)(x + (double)rand() / RAND_MAX) / film->resolution;
                float ny = (float)(y + (double)rand() / RAND_MAX) / film->resolution;
                
                Ray r = cam.generateRay(nx, ny);
                float minT = INFINITY;
                int i = 0;
                for (int j = 0; j < scene.size(); j++)
                {
                    float t = scene[j]->rayIntersection(r.origin, r.direction, j);
                    if (t < minT)
                    {
                        minT = t;
                        i = j;
                    }
                }
                if (minT < INFINITY)
                {
                    vec3 sampleColor = vec3(0, 0, 0);
                    vec3 pointLocal = r.origin + r.direction * minT;
                    vec3 lightDir = (pointLocal - light.position).unit();
                    if (scene[i]->getName() == "Sphere")
                    {
                        vec3 normal = (pointLocal - scene[i]->getCenter()).unit();
                        sampleColor = Radiance(light, pointLocal, normal, lightDir, scene, "Sphere", scene[i]->getColor());
                    }
                    if (scene[i]->getName() == "Plane")
                    {
                        vec3 normal = scene[i]->getNormal();
                        sampleColor = Radiance(light, pointLocal, normal, lightDir, scene, "Plane", scene[i]->getColor());
                    }
                    color += sampleColor;
                }
                if (minT == INFINITY)
                {
                    if (light.rayIntersection(r.origin, r.direction))
                    {
                        color += light.color * nSamples;
                    }
                }
            }
            color /= nSamples;
            film->setPixel(x, y, color);
        }
    }
    return film->pixels;
}
void benchmarkSamples(){
    const int width = 300;
    const int height = 300;
    Camera cam(vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 0, 1));
    cam.lookAt();
    cam.setScreen((float)height / width, 3.4 / 4, 4);
    Film film(width, 1,(float)height / width);

    vector<Shape*> scene;
    scene.push_back(new Sphere(0.5, 5,0,0, 1,0,0));
    scene.push_back(new Plane(vec3(0, 0, 1), -3,vec3(0,1,0)));
    scene.push_back(new Plane(vec3(0, 1, 0), -3,vec3(0,0.5,0.5)));

    vector<string> types = {"stratified","standard","normal","random","normalCenter"};

    for (int i = 0; i < types.size(); i++){
        RectangleLight light(vec3(2.8,-1,-1.5),vec3(-1,0,0),vec3(0,0.4,0),vec3(0,0,0.4),200);
        Render(scene, cam, &film,light,20,10,types[i]);
        std::string filename = "output" + std::to_string(i) + ".ppm";
        cout << filename << endl;
        ImgPPMExport(filename.c_str(), width, height, film.pixels);

    }

}
int main()
{
  
    const int width = 1200;
    const int height = 1200;
    Camera cam(vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 0, 1));
    cam.lookAt();
    cam.setScreen((float)height / width, 3.4 / 4, 4);
 
    Film film(width, 1, (float)height / width);

    vector<Shape *> scene;
    // esfera normal
      scene.push_back(new Sphere(0.5, 6,0,1, 1,0,0));
    //  esfera fosca
    //scene.push_back(new ReflectiveSphere(0.5, vec3(6, 0, 1), vec3(0, 0, 1)));
    // esfera metal
    // scene.push_back(new MetalSphere(0.5, vec3(6, 0, 1), vec3(0, 0, 1), vec3(0.2, 0.2, 0.2)));
    scene.push_back(new Plane(vec3(0, 0, 1), -3, vec3(0, 1, 0)));
    scene.push_back(new Plane(vec3(0, 1, 0), -3, vec3(0, 0.5, 0.5)));

    // Outros objetos e luzes...

    // scene.push_back(new Plane(vec3(-1, 0, 0), 9,vec3(0.8,0,1)));

    Light light(vec3(4, 0, 0), 1000);

    RectangleLight light2(vec3(4, 0, 0), vec3(-1, 0, 0), vec3(0, 0.5, 0), vec3(0, 0, 0.5), 10);

    

    Render(scene, cam, &film, light2, 20, 10, "stratified");

   

    ImgPPMExport("output.ppm", width, height, film.pixels);


    return 0;
}
