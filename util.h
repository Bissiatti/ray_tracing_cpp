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

void ImgPPMExport(const char* filename,int width, int height, const std::vector<vec3>& pixels) {
    std::ofstream ppmFile(filename);
    ppmFile << "P3\n" << width << " " << height << "\n255\n";

    for (int i = height - 1; i >= 0; --i) {
        for (int j = 0; j < width; ++j) {
            int index = i * width + j;
            int ir = int(255.99 * clamp(pixels[index].x, 0.0f, 1.0f));
            int ig = int(255.99 * clamp(pixels[index].y, 0.0f, 1.0f));
            int ib = int(255.99 * clamp(pixels[index].z, 0.0f, 1.0f));
            ppmFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    ppmFile.close();
}

// void ImgPPMExport(const char* filename, int width, int height, vector<vec3> data){

//     remove(filename);

//     FILE* file = fopen(filename, "w");
//     if (file == NULL) {
//         perror("Erro ao abrir o arquivo");
//         return;
//     }
//     fprintf(file, "P3\n%d %d\n255\n", width, height);
//     for (int i = 0; i < data.size(); i++) {
//         vec3 color = data[i];
//         // cout << color << endl;
//         int r = (int)(255.0 * clamp(color.x, 0.0f, 1.0f));
//         int g = (int)(255.0 * clamp(color.y, 0.0f, 1.0f));
//         int b = (int)(255.0 * clamp(color.z, 0.0f, 1.0f));
//         fprintf(file,"%d %d %d\n", r, g, b);
//     }
    
//     fclose(file);
    
// };