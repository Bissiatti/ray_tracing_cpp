#include <iostream>
#include "linalg.h"
#include "ray.h"
#pragma once

using namespace std;

class Shape{
public:
    virtual ~Shape() {} // Destrutor virtual para permitir destruição correta
    // ... outros métodos comuns a todas as formas
    virtual string getName() const { return "Shape"; }
    virtual vec3 getColor() const { }
    virtual vec3 getNormal() const = 0; // Método virtual puro
    virtual float rayIntersection(const vec3& origin, const vec3& direction) = 0;
    virtual float rayIntersection(const vec3& origin, const vec3& direction, int id) = 0;
    virtual float rayIntersection(const Ray& ray) = 0;
    virtual vec3 getCenter() const = 0; // Método virtual puro
};

class Plane : public Shape {
    private:
    vec3 color = vec3(0, 1, 0);
    public:
    vec3 normal;
    float d;
    string getName() const override { return "Plane"; }
    vec3 getColor() const override { return color;}
    vec3 getNormal() const { return normal; }
    Plane(const vec3& n, float d, vec3 c) : normal(n), d(d),  color(c){}
    Plane() : normal(vec3(0, 0, 1)), d(0) {}
    Plane(float a, float b, float c, float d) : normal(vec3(a, b, c)), d(d) {}

    float rayIntersection(const vec3& origin, const vec3& direction) {
        float t = -(normal.dot(origin) + d) / normal.dot(direction);
        if (t < 0) return INFINITY;
        return t;
    }

    float rayIntersection(const vec3& origin, const vec3& direction, int id) {
        float t = -(normal.dot(origin) + d) / normal.dot(direction);
        if (t < 0) return INFINITY;
        return t;
    }

    float rayIntersection(const Ray& ray) {
        return rayIntersection(ray.origin, ray.direction);
    }

    vec3 getCenter() const override { 
        // Implementação para Plane
        // Lançar uma exceção ou retornar um valor padrão
    }
};

class Sphere : public Shape {
    private:
    vec3 color = vec3(1, 0, 0);

    public:
    float radius;
    vec3 center;
    string name = "Sphere";
    vec3 getNormal() const override { return vec3(0, 0, 0); }
    vec3 getColor() const override { return color; }
    string getName() const override { return name; }
    Sphere(float r, vec3 c) : radius(r), center(c) {}
    Sphere() : radius(1), center(vec3(0, 0, 0)) {}
    Sphere(float r, float x, float y, float z,float R, float G, float B) : radius(r), center(vec3(x, y, z)), color(vec3(R,G,B)) {}

    vec3 getCenter() const override { return center; } // Implementação para Sphere


    float rayIntersection(const vec3& origin, const vec3& direction)
    {
        vec3 oc = origin - center;
        float a = direction.dot(direction);
        float b = 2.0 * oc.dot(direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        float root1 = (-b - sqrt(discriminant)) / (2.0 * a);
        if (discriminant < 0)
        {
            return INFINITY;
        }
        if (root1 > 0)
        {
            return root1;
        }

        else {
            // cout << "origin: " << origin << " direction: " << direction << endl;
            color = vec3(0, 0, 1);
            return (-b + sqrt(discriminant)) / (2.0 * a);
        }
    }

    float rayIntersection(const vec3& origin, const vec3& direction,int id)
    {
        vec3 oc = origin - center;
        float a = direction.dot(direction);
        float b = 2.0 * oc.dot(direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        if (discriminant < 0)
        {
            return INFINITY;
        }
        float root1 = (-b - sqrt(discriminant)) / (2.0 * a);
        if (root1 > 0)
        {
            // cout << "1 origin: " << origin << " direction: " << direction << " id " << id << endl;
            color = vec3(0, 100, 0);
            return root1;
        }

        else {
            // cout << root1 << " a " << a << " b " << b << " c " << c << " disc " << discriminant << endl;
            // cout << "2 origin: " << origin << " direction: " << direction << " id " << id << endl;
            color = vec3(0, 0, 100);
            // cout << (-b + sqrt(discriminant)) / (2.0 * a) << endl;
            return (-b + sqrt(discriminant)) / (2.0 * a);
        }
    }

    float rayIntersection(const Ray& ray)
    {
        return rayIntersection(ray.origin, ray.direction);
    }
};

ostream& operator<<(ostream& os, const Sphere& s)
{
    os << "Sphere(" << s.radius << ", " << s.center << ")";
    return os;
}