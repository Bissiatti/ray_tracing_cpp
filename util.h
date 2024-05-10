#include <iostream>
#include <fstream>
#include <vector>
#include "linalg.h"

using namespace std;

float clamp(float val, float min, float max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

float sigmoid(float x) {
    return 1.0f / (1.0f + exp(-x));
}

void ImgPPMExport(const char* filename,int width, int height, const vector<vec3>& pixels) {
    std::ofstream ppmFile(filename);
    ppmFile << "P3\n" << width << " " << height << "\n255\n";

    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();

    // Encontre os valores mínimo e máximo
    for (const auto& pixel : pixels) {
        min = std::min(min, std::min(pixel.x, std::min(pixel.y, pixel.z)));
        max = std::max(max, std::max(pixel.x, std::max(pixel.y, pixel.z)));
    }

    // Normalização Min-Max
    for (int i = height - 1; i >= 0; --i) {
        for (int j = 0; j < width; ++j) {
            int index = i * width + j;
            float r = (pixels[index].x - min) / (max - min);
            float g = (pixels[index].y - min) / (max - min);
            float b = (pixels[index].z - min) / (max - min);
            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);
            ppmFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    ppmFile.close();
}