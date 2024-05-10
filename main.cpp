#include <iostream>
#include <cmath>
#include <random>
using namespace std;
#include "linalg.h"
#include "ray.h"
#include "sphere.h"
#include "cam.h"
#include "util.h"
#include "film.h"
#include "light.h"
#include <vector>

std::default_random_engine generator; // Cria um gerador de números aleatórios

vec3 Radiance(Light light,vec3 pointLocal,vec3 normal,vec3 lightDir, vector<Shape*>& scene, string Type,vec3 colorObj){
    vec3 color;
    Ray rayLight = Ray(light.position, lightDir);
    // check if the light is visible from the point
    float minT = INFINITY;
    for (int j = 0; j < scene.size(); j++) {
        float t = scene[j]->rayIntersection(rayLight.origin, rayLight.direction, j);
        if (t < minT) {
            minT = t;
        }
    }
    if (minT == INFINITY) {
        return vec3(0.5, 0.5, 0.5);
    }
    vec3 point = rayLight.origin + rayLight.direction * minT;
    if ((point - pointLocal).length() < 0.00001) {
        if (Type == "Plane") {
            vec3 color1 = colorObj * light.color * std::max(0.0f, normal.dot(lightDir));
            vec3 color2 = colorObj * light.color * std::max(0.0f, normal.dot(lightDir*-1));
            color = color1.max(color2);
        }
        if (Type == "Sphere") {
            vec3 h = (lightDir - pointLocal).unit();
            color = colorObj * light.color * std::max(0.0f, normal.dot(h));
        }
    }
    color = color*light.intensity/(minT);
    return color;
}

vec3 Radiance(RectangleLight light,vec3 pointLocal,vec3 normal,vec3 lightDir, vec3 lightPos, vector<Shape*>& scene, string Type,vec3 colorObj){
    vec3 color;
    Ray rayLight = Ray(lightPos, lightDir);
    // check if the light is visible from the point
    float minT = INFINITY;
    for (int j = 0; j < scene.size(); j++) {
        float t = scene[j]->rayIntersection(rayLight.origin, rayLight.direction, j);
        if (t < minT) {
            minT = t;
        }
    }
    if (minT == INFINITY) {
        return vec3(0, 0, 0);
    }
    vec3 point = rayLight.origin + rayLight.direction * minT;
    if ((point - pointLocal).length() < 0.00001) {
        if (Type == "Plane") {
            vec3 color1 = colorObj * light.color * std::max(0.0f, normal.dot(lightDir));
            vec3 color2 = colorObj * light.color * std::max(0.0f, normal.dot(lightDir*(-1)));
            color = color1.max(color2);
        }
        if (Type == "Sphere") {
            vec3 h = (lightDir - pointLocal).unit();
            color = colorObj * light.color * std::max(0.0f, normal.dot(h));
        }
    }
    color = color*light.intensity/(minT);
    return color;
}

vector<vec3> Render(vector<Shape*>& scene, Camera cam, Film *film, RectangleLight light, int nSamples,int nSamplesLight, string typeSamples) {
    int maxY = film->resolution*cam.ratio;
    film->pixels = vector<vec3>(maxY*film->resolution);
    cout << maxY << " " << film->resolution << endl; 
    int m1 = sqrt(nSamplesLight);
    int m2 = sqrt(nSamplesLight)+1;
    int m3 = m1*m2;
    cout << m1 << " " << m2 << " " << m3 << endl;
    float dx = 1.0/m1;
    float dy = 1.0/m2;
    cout << dx << " " << dy << endl;
    for (int y = 0; y < maxY; y++) {
        for (int x = 0; x < film->resolution; x++) {
            vec3 color = vec3(0, 0, 0);
            for (int sample = 0; sample < nSamples*nSamplesLight; sample++) {   

                float lightX;
                float lightY;
                     
                if(typeSamples == "stratified"){
                    lightX = ((sample % m3) % m1)*dx + (double)rand() / RAND_MAX * dx;
                    lightY = ((sample % m3)/m1)*dy + (double)rand() / RAND_MAX * dy;       
                }
                else if (typeSamples == "standard"){
                    lightX = ((sample % m3) % m1)*dx + dx/2;
                    lightY = ((sample % m3)/m1)*dy + dy/2;
                }
                else if (typeSamples == "normal"){
                    lightX = normal_distribution<double>(((sample % m3)%m1)*dx + dx/2,dx/2)(generator);
                    lightY = normal_distribution<double>(((sample % m3)/m1)*dy + dy/2,dy/2)(generator);
                }
                else if (typeSamples == "normalCenter"){
                    lightX = normal_distribution<double>(0.5 + 0.15)(generator);
                    lightY = normal_distribution<double>(0.5 + 0.15)(generator);
                }
                else{
                    lightX = (double)rand() / RAND_MAX;
                    lightY = (double)rand() / RAND_MAX;
                }    

                float nx = (float)(x + (double)rand() / RAND_MAX) / film->resolution;
                float ny = (float)(y + (double)rand() / RAND_MAX) / film->resolution;
                // cout << nx << " " << ny << endl;
                Ray r = cam.generateRay(nx, ny);
                float minT = INFINITY;
                int i = 0;
                for (int j = 0; j< scene.size(); j++) {
                    float t = scene[j]->rayIntersection(r.origin, r.direction,j);
                    if (t < minT) {
                        minT = t;
                        i = j;
                    }
                }
                if (minT < INFINITY) {
                    vec3 sampleColor = vec3(0, 0, 0); 
                    vec3 pointLocal = r.origin + r.direction * minT;
                    vec3 lightDir = (pointLocal - (light.position + light.width*lightX + light.height*lightY)).unit();
                    vec3 lightPos = light.position + light.width*lightX + light.height*lightY;
                    if(scene[i]->getName() == "Sphere"){
                        vec3 normal = (pointLocal - scene[i]->getCenter()).unit();
                        sampleColor = Radiance(light,pointLocal,normal,lightDir,lightPos,scene,"Sphere",scene[i]->getColor());
                    }
                    if (scene[i]->getName() == "Plane") {
                        vec3 normal = scene[i]->getNormal();
                        sampleColor = Radiance(light,pointLocal,normal,lightDir,lightPos,scene,"Plane",scene[i]->getColor());
                    }
                    color += sampleColor;
                }
                if (minT == INFINITY) {
                    if (light.rayIntersectionArea(r.origin, r.direction)) {
                        color += light.color*nSamples;
                    }
                }
            }
            color /= nSamples;
            color /= nSamplesLight;
            color *= light.area;
            film->setPixel(x,y,color);
        }
    }
    return film->pixels;
}

vec3 Intersect(vector<Shape*>& scene, Ray r, Light light, int nSamples, vec3 color, int depth = 0){
    float minT = INFINITY;
    int i = 0;
    for (int j = 0; j< scene.size(); j++) {
        float t = scene[j]->rayIntersection(r.origin, r.direction,j);
        if (t < minT && abs(t) > 0.1) {
            minT = t;
            i = j;
        }
    }
    if (minT < INFINITY) {
        vec3 sampleColor = vec3(1, 1, 1); 
        vec3 pointLocal = r.origin + r.direction * minT;
        vec3 lightDir = (pointLocal - light.position).unit();
        if(scene[i]->getName() == "Sphere"){
            vec3 normal = (pointLocal - scene[i]->getCenter()).unit();
            if (scene[i]->getType() == "Metal"){
                vec3 reflectDir = reflect(r.direction,normal);
                if (depth < 5){
                    vec3 reflectedColor = Intersect(scene,Ray(pointLocal,reflectDir),light,nSamples,color,depth + 1);
                    sampleColor += scene[i]->getColor() * reflectedColor; // Multiplica a cor do objeto pela cor refletida
                }
                else{
                    return vec3(0,0,0);
                }
            }
                sampleColor += Radiance(light,pointLocal,normal,lightDir,scene,"Sphere",scene[i]->getColor());

        }
        if (scene[i]->getName() == "Plane") {
            vec3 normal = scene[i]->getNormal();
            if (scene[i]->getType() == "Metal"){
                vec3 reflectDir = reflect(r.direction,normal);
                if (depth < 5){
                    vec3 reflectedColor = Intersect(scene,Ray(pointLocal,reflectDir),light,nSamples,color,depth + 1);
                    sampleColor += scene[i]->getColor() * reflectedColor; // Multiplica a cor do objeto pela cor refletida

                }
                else{
                    return vec3(0,0,0);
                }
            }
            sampleColor += Radiance(light,pointLocal,normal,lightDir,scene,"Plane",scene[i]->getColor());

            
        }
        color += sampleColor;
    }
    if (minT == INFINITY) {
        if (light.rayIntersection(r.origin, r.direction)) {
            color += light.color*nSamples;
        }
    }
    return color;
}

vector<vec3> Render(vector<Shape*>& scene, Camera cam, Film *film, Light light, int nSamples) {
    int maxY = film->resolution*cam.ratio;
    film->pixels = vector<vec3>(maxY*film->resolution);
    for (int y = 0; y < maxY; y++) {
        for (int x = 0; x < film->resolution; x++) {
            vec3 color = vec3(0, 0, 0);
            for (int sample = 0; sample < nSamples; sample++) {
                float nx = (float)(x + (double)rand() / RAND_MAX) / film->resolution;
                float ny = (float)(y + (double)rand() / RAND_MAX) / film->resolution;
                // cout << nx << " " << ny << endl;
                Ray r = cam.generateRay(nx, ny);
                color = Intersect(scene,r,light,nSamples,color);
            }
            color /= nSamples;
            film->setPixel(x,y,color);
        }
    }
    return film->pixels;
}

// void benchmarkSamples(){
//     const int width = 300;
//     const int height = 300;
//     Camera cam(vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 0, 1));
//     cam.lookAt();
//     cam.setScreen((float)height / width, 3.4 / 4, 4);
//     Film film(width, 1,(float)height / width);

//     vector<Shape*> scene;
//     scene.push_back(new Sphere(0.5, 5,0,0, 1,0,0));
//     scene.push_back(new Plane(vec3(0, 0, 1), -3,vec3(0,1,0)));
//     scene.push_back(new Plane(vec3(0, 1, 0), -3,vec3(0,0.5,0.5)));

//     vector<string> types = {"stratified","standard","normal","random","normalCenter"};

//     for (int i = 0; i < types.size(); i++){
//         RectangleLight light(vec3(2.8,-1,-1.5),vec3(-1,0,0),vec3(0,0.4,0),vec3(0,0,0.4),50);
//         Render(scene, cam, &film,light,20,10,types[i]);
//         std::string filename = "output" + std::to_string(i) + ".ppm";
//         cout << filename << endl;
//         ImgPPMExport(filename.c_str(), width, height, film.pixels);

//     }

// }

int main() {
    cout << "Luz pontual" << endl;
    const int width = 300;
    const int height = 300;
    Camera cam(vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 0, 1));
    cam.lookAt();
    cam.setScreen((float)height / width, 3.4 / 4, 4);
    cout << cam << endl;
    Film film(width, 1,(float)height / width);

    vector<Shape*> scene;
    scene.push_back(new Sphere(0.5, 4,0,0, 1,0,0, "Metal"));
    scene.push_back(new Plane(vec3(0, 0, 1), -1.5,vec3(0.5,0.5,0.5), "Standard"));
    scene.push_back(new Plane(vec3(0, 1, 0), -1.5,vec3(0,0.5,0.5), "Standard"));
    scene.push_back(new Plane(vec3(0, 1, 0), 1.5,vec3(0.5,0,0.5), "Standard"));


    Light light(vec3(2,-1,-1.5),32);

    cout << scene.size() << endl;

    Render(scene, cam, &film,light,100);

    cout << height*width <<endl;
    cout << film.resolution*film.resolution*cam.ratio << endl;
    cout << film.pixels.size() << endl;

    ImgPPMExport("output.ppm", width, height, film.pixels);

    // cout << "Luz retangular" << endl;
    
    // benchmarkSamples();

    return 0;

}

