#include <iostream>
#include "linalg.h"
#include "ray.h"
#pragma once

using namespace std;

struct Radiance{
    vec3 L;
    vec3 I;
};

class Light
{
    public:
    vec3 position;
    float intensity;
    vec3 color = vec3(1, 1, 1);

    Light(const vec3& p, float i) : position(p), intensity(i) {}

    void setColor(const vec3& color)
    {
        this->color = color;
    }

    void setColor(float r, float g, float b)
    {
        this->color = vec3(r, g, b);
    }

    bool rayIntersection(const vec3& origin, const vec3& direction)
    {
        vec3 v = position - origin;

        float i = direction.imaxAbs();
        if (v.get(i) == 0) return false;
        float t = (v.get(i) / direction.get(i));
        if (t < 0) return false;
        // cout << abs(v.x - t * direction.x) << " " << abs(v.y - t * direction.y) << " " << abs(v.z - t * direction.z) << endl;
        return std::abs(v.y - t * direction.y) < 0.001 && std::abs(v.z - t * direction.z) < 0.001;
    }

    bool rayIntersection(const Ray& ray)
    {
        return rayIntersection(ray.origin, ray.direction);
    }

    
    vec3 radiance(const vec3& origin){
        float r = (position - origin).length();
        vec3 L = position/r;
        return L;
    }
};  

