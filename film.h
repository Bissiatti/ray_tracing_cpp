#include <iostream>
#include <vector>
#include "linalg.h"


#pragma once

using namespace std;

class Film
{
    public:
    int resolution;
    int samplesPerPixel = 1;
    float ratio;
    vector<vec3> pixels;
    Film(int resolution, int samplesPerPixel, float ratio) : resolution(resolution), samplesPerPixel(samplesPerPixel)
    {
        pixels = vector<vec3>(resolution*ratio*resolution);
    }

    void setPixel(int x, int y, const vec3& color)
    {
        pixels[y * resolution + x] = color;
    }

};