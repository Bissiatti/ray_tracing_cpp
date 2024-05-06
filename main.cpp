#include <iostream>
using namespace std;
#include "linalg.h"
#include "ray.h"
#include "sphere.h"
#include "cam.h"
#include "util.h"
#include "film.h"
#include "light.h"
#include <vector>

vec3 Radiance(Light light,vec3 pointLocal,vec3 normal,vec3 lightDir,vector<Shape*>& scene, string Type){
    vec3 color = vec3(0,0,0);
    Ray rayLight = Ray(light.position, lightDir);
    // check if the light is visible from the point
    float minT = INFINITY;
    for (int j = 0; j < scene.size(); j++) {
        float t = scene[j]->rayIntersection(rayLight.origin, rayLight.direction, j);
        if (t < minT) {
            minT = t;
            // if (j == 1){
            //     cout << "plane" << t<< rayLight.origin + rayLight.direction * t << endl;
            // }
        }
    }
    if (minT == INFINITY) {
        return vec3(0, 0, 0);
    }
    vec3 point = rayLight.origin + rayLight.direction * minT;
    if ((point - pointLocal).length() < 0.00001) {
        if (Type == "Plane") {
            color = light.color * std::max(0.0f, normal.dot(lightDir));
        }
        if (Type == "Sphere") {
            vec3 h = (lightDir - pointLocal).unit();
            color = light.color * std::max(0.0f, normal.dot(h));
        }
    }
    return color;
}

vector<vec3> Render(vector<Shape*>& scene, Camera cam, Film *film,Light light) {
    int maxY = film->resolution*cam.ratio;
    film->pixels = vector<vec3>(maxY*film->resolution);
    cout << maxY << " " << film->resolution << endl; 
    for (int y = 0; y < maxY; y++) {
        for (int x = 0; x < film->resolution; x++) {
            float nx = (float)x / film->resolution;
            float ny = (float)y / film->resolution;
            Ray r = cam.generateRay(nx, ny);
            float minT = INFINITY;
            vec3 color = vec3(0, 0, 0);
            int i = 0;
            for (int j = 0; j< scene.size(); j++) {
                float t = scene[j]->rayIntersection(r.origin, r.direction,j);
                if (t < minT) {
                    minT = t;
                    i = j;
                }
            }
            if (minT < INFINITY) {
                color = scene[i]->getColor();
                vec3 pointLocal = r.origin + r.direction * minT;
                if(scene[i]->getName() == "Sphere"){
                    vec3 normal = (pointLocal - scene[i]->getCenter()).unit();
                    vec3 lightDir = (pointLocal - light.position).unit();
                    // color = vec3(normal.x+1, normal.y+1, normal.z+1)*0.5;
                    color = Radiance(light,pointLocal,normal,lightDir,scene,"Sphere");
                }
                if (scene[i]->getName() == "Plane") {
                    vec3 normal = scene[i]->getNormal();
                    vec3 lightDir = (pointLocal - light.position).unit();
                    color = Radiance(light,pointLocal,normal,lightDir,scene,"Plane");
                }
            }
            if (minT == INFINITY) {
                if (light.rayIntersection(r.origin, r.direction)) {
                    cout << "light" << endl;
                    color = light.color;
                }
            }
            film->setPixel(x,y,color);
        }
    }
    return film->pixels;
}

int main() {
    cout << "Hello, World!" << endl;
    const int width = 300;
    const int height = 200;
    Camera cam(vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 0, 1));
    cam.lookAt();
    cam.setScreen((float)height / width, 3.1415 / 4, 1);
    cout << cam << endl;
    Film film(width, 1,(float)height / width);

    vector<Shape*> scene;
    scene.push_back(new Sphere(0.5, 5, 0, 1));
    scene.push_back(new Plane(vec3(0, 0, 1), -3));

    Light light(vec3(4,0,0),1);

    cout << scene.size() << endl;

    Render(scene, cam, &film,light);

    cout << height*width <<endl;
    cout << film.resolution*film.resolution*cam.ratio << endl;
    cout << film.pixels.size() << endl;

    ImgPPMExport("output.ppm", width, height, film.pixels);
    return 0;
}

