#include <iostream>   // Inclui a biblioteca de entrada e saída padrão
#include "linalg.h"   // Inclui a biblioteca de álgebra linear
#pragma once        // Garante que este arquivo seja incluído apenas uma vez durante a compilação

using namespace std;  // Usa o namespace std para evitar repetição de std::

class Ray
{
public:
    vec3 origin;      // Origem do raio, representada por um vetor tridimensional
    vec3 direction;   // Direção do raio, representada por um vetor tridimensional

    // Construtor da classe Ray que aceita origem e direção como parâmetros
    Ray(const vec3& o, const vec3& d) : origin(o), direction(d.unit()) {}

    // Construtor padrão da classe Ray que define a origem como (0,0,0) e a direção como (0,0,1)
    Ray() : origin(vec3(0, 0, 0)), direction(vec3(0, 0, 1)) {}

    // Construtor da classe Ray que aceita coordenadas separadas para origem e direção
    Ray(float ox, float oy, float oz, float dx, float dy, float dz) : origin(vec3(ox, oy, oz)), direction(vec3(dx, dy, dz)) {}
};