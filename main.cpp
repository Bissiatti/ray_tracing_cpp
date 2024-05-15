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

// Cacula a cor refletida por um objeto em relação a uma luz pontual
vec3 Radiance(Light light,vec3 pointLocal,vec3 normal,vec3 lightDir, vector<Shape*>& scene, string Type,vec3 colorObj){
    vec3 color;
    Ray rayLight = Ray(light.position, lightDir);
    // Checa se a luz é visível a partir do ponto
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
        // Se o ponto é visível, calcula a cor
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

// Cacula a cor refletida por um objeto em relação a uma luz retangular
vec3 Radiance(RectangleLight light,vec3 pointLocal,vec3 normal,vec3 lightDir, vec3 lightPos, vector<Shape*>& scene, string Type,vec3 colorObj){
    vec3 color;
    Ray rayLight = Ray(lightPos, lightDir);
    // Checa se a luz é visível a partir do ponto
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
        // Se o ponto é visível, calcula a cor
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

// Calcula a interseção de um raio com os objetos da cena
vec3 Intersect(vector<Shape*>& scene, Ray r, Light light, int nSamples, int depth = 0){
    vec3 color = vec3(0,0,0);
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
                    vec3 reflectedColor = Intersect(scene,Ray(pointLocal,reflectDir),light,nSamples,depth + 1);
                    sampleColor += scene[i]->getColor() * reflectedColor;
                }
                else{
                    return vec3(0,0,0);
                }
            }
            else if (scene[i]->getType() == "Diffuse"){
                vec3 randomDir = random_on_hemisphere(normal);
                if (depth < 5){
                vec3 reflectedColor = Intersect(scene,Ray(pointLocal,randomDir),light,nSamples,depth + 1);
                sampleColor += scene[i]->getColor() * reflectedColor;
                }
                else {
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
                    vec3 reflectedColor = Intersect(scene,Ray(pointLocal,reflectDir),light,nSamples,depth + 1);
                    sampleColor += scene[i]->getColor() * reflectedColor; // Multiplica a cor do objeto pela cor refletida

                }
                else{
                    return vec3(0,0,0);
                }
            }
            else if (scene[i]->getType() == "Diffuse"){
                if (depth < 5){
                vec3 randomDir = random_on_hemisphere(normal);
                vec3 reflectedColor = Intersect(scene,Ray(pointLocal,randomDir),light,nSamples,depth + 1);
                sampleColor += scene[i]->getColor() * reflectedColor;
                }
                else {
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

vec3 Intersect(vector<Shape*>& scene, Ray r, RectangleLight light, vec3 lightPos, int nSamples, int depth = 0){
    vec3 color = vec3(0,0,0);
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
        vec3 lightDir = (pointLocal - lightPos).unit();
        if(scene[i]->getName() == "Sphere"){
            vec3 normal = (pointLocal - scene[i]->getCenter()).unit();
            if (scene[i]->getType() == "Metal"){
                vec3 reflectDir = reflect(r.direction,normal);
                if (depth < 5){
                    vec3 reflectedColor = Intersect(scene,Ray(pointLocal,reflectDir),light,lightPos,nSamples,depth + 1);
                    sampleColor += scene[i]->getColor() * reflectedColor; // Multiplica a cor do objeto pela cor refletida
                }
                else{
                    return vec3(0,0,0);
                }
            }
            else if (scene[i]->getType() == "Diffuse"){
                vec3 randomDir = random_on_hemisphere(normal);
                if (depth < 5){
                vec3 reflectedColor = Intersect(scene,Ray(pointLocal,randomDir),light,lightPos,nSamples,depth + 1);
                sampleColor += scene[i]->getColor() * reflectedColor;
                }
                else {
                    return vec3(0,0,0);
                }
            }
            sampleColor += Radiance(light,pointLocal,normal,lightDir,lightPos,scene,"Sphere",scene[i]->getColor());

        }
        if (scene[i]->getName() == "Plane") {
            vec3 normal = scene[i]->getNormal();
            if (scene[i]->getType() == "Metal"){
                vec3 reflectDir = reflect(r.direction,normal);
                if (depth < 5){
                    vec3 reflectedColor = Intersect(scene,Ray(pointLocal,reflectDir),light,lightPos,nSamples,depth + 1);
                    sampleColor += scene[i]->getColor() * reflectedColor; // Multiplica a cor do objeto pela cor refletida

                }
                else{
                    return vec3(0,0,0);
                }
            }
            else if (scene[i]->getType() == "Diffuse"){
                if (depth < 5){
                vec3 randomDir = random_on_hemisphere(normal);
                vec3 reflectedColor = Intersect(scene,Ray(pointLocal,randomDir),light,lightPos,nSamples,depth + 1);
                sampleColor += scene[i]->getColor() * reflectedColor;
                }
                else {
                    return vec3(0,0,0);
                }
            }
            sampleColor += Radiance(light,pointLocal,normal,lightDir,lightPos,scene,"Plane",scene[i]->getColor());
            
        }
        color += sampleColor;
    }
    if (minT == INFINITY) {
        if (light.rayIntersectionArea(r.origin, r.direction)) {
            color += light.color*nSamples;
        }
    }
    return color;
}

vector<vec3> Render(vector<Shape*>& scene, Camera cam, Film *film, RectangleLight light, int nSamples,int nSamplesLight, string typeSamples) {
    int maxY = film->resolution*cam.ratio;
    film->pixels = vector<vec3>(maxY*film->resolution);
    int m1 = sqrt(nSamplesLight);
    int m2 = sqrt(nSamplesLight)+1;
    int m3 = m1*m2;
    float dx = 1.0/m1;
    float dy = 1.0/m2;
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

                vec3 lightPos = light.position + light.width*lightX + light.height*lightY;

                color = Intersect(scene,r,light,lightPos,nSamples);


            }
            color /= nSamples;
            color /= nSamplesLight;
            color *= light.area;
            film->setPixel(x,y,color);
        }
    }
    return film->pixels;
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
                color = Intersect(scene,r,light,nSamples);
            }
            color /= nSamples;
            film->setPixel(x,y,color);
        }
    }
    return film->pixels;
}

void benchmarkSamples(){
    const int width = 600;
    const int height = 600;
    Camera cam(vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 0, 1));
    cam.lookAt();
    cam.setScreen((float)height / width, 3.4 / 4, 4);
    Film film(width, 1,(float)height / width);

    vector<Shape*> scene;
    scene.push_back(new Sphere(0.5, 4,0,0, 1,0,0, "Diffuse"));
    scene.push_back(new Plane(vec3(0, 0, 1), -1.5,vec3(0.5,0.5,0.5), "Metal"));
    scene.push_back(new Plane(vec3(0, 1, 0), -1.5,vec3(0,0.5,0.5), "Metal"));
    scene.push_back(new Plane(vec3(0, 1, 0), 1.5,vec3(0.5,0,0.5), "Standard"));

    vector<string> types = {"stratified","standard","normal","random","normalCenter"};

    for (int i = 0; i < types.size(); i++){
        RectangleLight light(vec3(2.8,-1,-1.5),vec3(-1,0,0),vec3(0,0.4,0),vec3(0,0,0.4),50);
        Render(scene, cam, &film,light,25,25,types[i]);
        std::string filename = "output" + std::to_string(i) + ".ppm";
        cout << filename << endl;
        ImgPPMExport(filename.c_str(), width, height, film.pixels);

    }

}

int main() {
    const int width = 600;
    const int height = 600;
    // definição da ceamera
    Camera cam(vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 0, 1));
    cam.lookAt();
    cam.setScreen((float)height / width, 3.4 / 4, 4);

    // definição do filme
    Film film(width, 1,(float)height / width);

    // definição dos objetos da cena
    vector<Shape*> scene;
    scene.push_back(new Sphere(0.5, 4,0,0, 1,0,0, "Diffuse"));
    scene.push_back(new Plane(vec3(0, 0, 1), -1.5,vec3(0.8,0.8,0.8), "Metal"));
    scene.push_back(new Plane(vec3(0, 1, 0), -1.5,vec3(0,0.5,0.5), "Metal"));
    scene.push_back(new Plane(vec3(0, 1, 0), 1.5,vec3(0.5,0,0.5), "Standard"));

    // definição da luz pontual

    Light light(vec3(2,-1,-1.5),32);

    // Renderização da cena

    cout << "Luz pontual" << endl;
    Render(scene, cam, &film,light,100);

    // Definição da luz retangular

    // RectangleLight light(vec3(2.8,-1,-1.5),vec3(-1,0,0),vec3(0,0.4,0),vec3(0,0,0.4),50);

    // Renderização da cena

    // cout << "Luz em área" << endl;
    // Render(scene, cam, &film,light,10,10,"stratified");


    // Exportação da imagem

    ImgPPMExport("output.ppm", width, height, film.pixels);

    cout << "output.ppm" << endl;

    // cout << "Luz em área benchmark" << endl;
    
    // benchmarkSamples();

    return 0;

}

