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

class RectangleLight
{
    public:
    vec3 normal;
    vec3 width;
    vec3 height;
    vec3 position;
    float area;
    float intensity;
    vec3 color = vec3(1, 1, 1);

    RectangleLight(const vec3& p, const vec3& n, vec3 w, vec3 h, float i) : position(p), normal(n), width(w), height(h), intensity(i) {this->area = w.length() * h.length();};

    void setColor(const vec3& color)
    {
        this->color = color;
    }

    void setColor(float r, float g, float b)
    {
        this->color = vec3(r, g, b);
    }
    
    bool rayIntersectionArea(const vec3& origin, const vec3& direction) {
        vec3 v = position - origin;
        float d = normal.dot(direction);

        // Se d for 0, então a direção é paralela ao plano do paralelogramo
        if (fabs(d) < 1e-6) return false;

        float t = normal.dot(v) / d;
        // Se t for negativo, a interseção está atrás do ponto de origem
        if (t < 0) return false;

        // Ponto de interseção com o plano
        vec3 P = origin + direction * t;

        // Verificar se P está dentro do paralelogramo
        vec3 v1 = P - position;
        float d1 = v1.dot(width);
        float d2 = v1.dot(height);

        // Se d1 e d2 estão entre 0 e o comprimento dos vetores, então P está dentro do paralelogramo
        if (d1 >= 0 && d1 <= width.length() && d2 >= 0 && d2 <= height.length())
            return true;

        return false;
    }


    vec3 radiance(const vec3& origin){
        float r = (position - origin).length();
        vec3 L = position/r;
        return L;
    }
 

};