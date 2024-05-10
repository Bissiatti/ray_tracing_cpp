#include <iostream> // Biblioteca de entrada e saída padrão
#include <vector>   // Biblioteca para trabalhar com vetores
#include "linalg.h" // Biblioteca de álgebra linear

#pragma once      // Garante que este arquivo seja incluído apenas uma vez durante a compilação

using namespace std; // Usa o namespace std para evitar repetição de std::

class Film
{
public:
    int resolution;            // Resolução do filme
    int samplesPerPixel = 1;   // Amostras por pixel
    float ratio;               // Razão de aspecto
    vector<vec3> pixels;       // Vetor de pixels representando a imagem

    // Construtor da classe Film
    Film(int resolution, int samplesPerPixel, float ratio) : resolution(resolution), samplesPerPixel(samplesPerPixel)
    {
        // Inicializa o vetor de pixels com o tamanho necessário
        pixels = vector<vec3>(resolution * ratio * resolution);
    }

    // Método para definir a cor de um pixel na posição (x, y)
    void setPixel(int x, int y, const vec3 &color)
    {
        // Calcula o índice correspondente ao pixel na matriz unidimensional de pixels
        pixels[y * resolution + x] = color;
    }

    // Método para tornar a textura da esfera fosca
    void makeSphereTextureMatte()
    {
        // Percorre todos os pixels da imagem
        for (int i = 0; i < pixels.size(); ++i)
        {
            // Torna a cor do pixel mais escura (fosca) multiplicando-a por um fator de atenuação
            // Isso simula uma textura fosca na superfície da esfera
            pixels[i] *= 0.5; // Pode ajustar o valor conforme necessário para a aparência desejada
        }
    }
};