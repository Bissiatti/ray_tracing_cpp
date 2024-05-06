#include <iostream>
#include "linalg.h"
#pragma once

using namespace std;

class Ray
{
    public:
    vec3 origin;
    vec3 direction;
    Ray(const vec3& o, const vec3& d) : origin(o), direction(d.unit()) {}
    Ray() : origin(vec3(0, 0, 0)), direction(vec3(0, 0, 1)) {}
    Ray(float ox, float oy, float oz, float dx, float dy, float dz) : origin(vec3(ox, oy, oz)), direction(vec3(dx, dy, dz)) {}
};