#include <iostream>   // Biblioteca de entrada e saída padrão
#include "linalg.h"   // Biblioteca de álgebra linear
#include "ray.h" 
#include "util.h"      // Inclui a definição da classe Ray
#pragma once        // Garante que este arquivo seja incluído apenas uma vez durante a compilação

using namespace std;  // Usa o namespace std para evitar repetição de std::

// Classe base para formas geométricas
class Shape {
public:
    virtual ~Shape() {} // Destrutor virtual para permitir destruição correta
    virtual string getName() const { return "Shape"; } // Retorna o nome da forma
    virtual vec3 getColor() const { } // Retorna a cor da forma
    virtual vec3 getAtt() const {}
    virtual vec3 getNormal() const = 0; // Retorna o vetor normal à forma
    virtual string getType() const {}
    virtual float rayIntersection(const vec3& origin, const vec3& direction) = 0; // Calcula a interseção entre um raio e a forma
    virtual float rayIntersection(const vec3& origin, const vec3& direction, int id) = 0; // Sobrecarga do método anterior com um identificador adicional
    virtual float rayIntersection(const Ray& ray) = 0; // Sobrecarga do método anterior usando um objeto Ray
    virtual vec3 getCenter() const = 0; // Retorna o centro da forma
};

// Classe que representa um plano
class Plane : public Shape {
private:
    const vec3 color; // Cor do plano
public:
    string type = "Standard";
    vec3 normal; // Vetor normal ao plano
    float d; // Distância do plano à origem
    Plane(const vec3& n, float d, vec3 c, string t) : normal(n), d(d), color(c), type(t) {} // Construtor com parâmetros
    Plane() : normal(vec3(0, 0, 1)), d(0), color(0,1,0) {} // Construtor padrão
    Plane(float a, float b, float c, float d,float R, float G, float B, string t) : normal(vec3(a, b, c)), d(d), color(vec3(R,G,B)), type(t) {} // Construtor com coordenadas separadas e cor

    // Métodos override da classe Shape
    string getName() const override { return "Plane"; } // Retorna o nome do plano
    vec3 getColor() const override { return color;} // Retorna a cor do plano
    vec3 getNormal() const override { return normal; } // Retorna o vetor normal ao plano
    string getType() const { return type; } // Retorna o tipo do plano

    // Métodos para interseção de raios com o plano
    float rayIntersection(const vec3& origin, const vec3& direction) override {
        float t = -(normal.dot(origin) + d) / normal.dot(direction);
        if (t < 0) return INFINITY;
        return t;
    }

    float rayIntersection(const vec3& origin, const vec3& direction, int id) override {
        float t = -(normal.dot(origin) + d) / normal.dot(direction);
        if (t < 0) return INFINITY;
        return t;
    }

    float rayIntersection(const Ray& ray) override {
        return rayIntersection(ray.origin, ray.direction);
    }

    vec3 getCenter() const override { 
        // Implementação para Plane
        // Lançar uma exceção ou retornar um valor padrão
    }
};

// Classe que representa uma esfera
class Sphere : public Shape {
private:
    const vec3 color; // Cor da esfera
public:
    float radius; // Raio da esfera
    vec3 center; // Centro da esfera
    string name = "Sphere"; // Nome da esfera
    string type = "Standard";

    // Construtores da classe Sphere
    Sphere(float r, vec3 c,vec3 rgb, string t) : radius(r), center(c),color(rgb), type(t) {} // Construtor com parâmetros
    Sphere() : radius(1), center(vec3(0, 0, 0)), color(vec3(1,0,0)) {} // Construtor padrão
    Sphere(float r, float x, float y, float z,float R, float G, float B, string t) : radius(r), center(vec3(x, y, z)), color(vec3(R,G,B)), type(t) {} // Construtor com coordenadas separadas e cor

    // Métodos override da classe Shape
    vec3 getNormal() const override { return vec3(0, 0, 0); } // Retorna o vetor normal à esfera
    vec3 getColor() const override { return color; } // Retorna a cor da esfera
    string getName() const override { return name; } // Retorna o nome da esfera
    string getType() const { return type; } // Retorna o tipo da esfera

    // Métodos para interseção de raios com a esfera
    float rayIntersection(const vec3& origin, const vec3& direction) override {
        vec3 oc = origin - center;
        float a = direction.dot(direction);
        float b = 2.0 * oc.dot(direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        float root1 = (-b - sqrt(discriminant)) / (2.0 * a);
        if (discriminant < 0) {
            return INFINITY;
        }
        if (root1 > 0) {
            return root1;
        }
        else {
            return (-b + sqrt(discriminant)) / (2.0 * a);
        }
    }

    float rayIntersection(const vec3& origin, const vec3& direction,int id) override {
        vec3 oc = origin - center;
        float a = direction.dot(direction);
        float b = 2.0 * oc.dot(direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        if (discriminant < 0) {
            return INFINITY;
        }
        float root1 = (-b - sqrt(discriminant)) / (2.0 * a);
        if (root1 > 0) {
            return root1;
        }
        else {
            return (-b + sqrt(discriminant)) / (2.0 * a);
        }
    }

    float rayIntersection(const Ray& ray) override {
        return rayIntersection(ray.origin, ray.direction);
    }

    vec3 getCenter() const override { return center; } // Retorna o centro da esfera
};

class ReflectiveSphere : public Sphere {
public: 
    string name = "ReflectiveSphere"; // Nome da esfera
    ReflectiveSphere(float r, const vec3& c, const vec3& rgb) : Sphere(r, c, rgb, "Reflective") {} // Construtor
    string getName() const override { return name; } // Retorna o nome da esfera
    // Método para interseção de raios com a esfera fosca
    float rayIntersection(const vec3& origin, const vec3& direction) override {
        vec3 oc = origin - center;
        float a = direction.dot(direction);
        float b = 2.0 * oc.dot(direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        if (discriminant < 0) {
            return INFINITY;
        }
        float root1 = (-b - sqrt(discriminant)) / (2.0 * a);
        if (root1 > 0) {
            return root1;
        }
        else {
            return (-b + sqrt(discriminant)) / (2.0 * a);
        }
    }

    // Método para interseção de raios com a esfera fosca, sobrecarregado com identificador adicional
    float rayIntersection(const vec3& origin, const vec3& direction, int id) override {
        return rayIntersection(origin, direction);
    }

    // Método para interseção de raios com a esfera fosca, sobrecarregado para usar objeto Ray
    float rayIntersection(const Ray& ray) override {
        return rayIntersection(ray.origin, ray.direction);
    }

    // Retorna o centro da esfera fosca
    vec3 getCenter() const override { return center; }
};



// Sobrecarga do operador de inserção em stream para a classe Sphere
ostream& operator<<(ostream& os, const Sphere& s) {
    os << "Sphere(" << s.radius << ", " << s.center << ")";
    return os;
}
