#include <iostream>
#include "linalg.h"
#include "ray.h"
#pragma once

using namespace std;

class Camera{
    public:
    vec3 eye;
    vec3 center;
    vec3 up;
    vec3 localOrigin = vec3(0, 0, 0);
    vec3 w;
    vec3 u;
    vec3 v;
    matrix3 rotationMatrix;
    matrix3 rotationMatrixInv;

    float ratio;
    float focalLength;
    vec3 screenTopLeft;
    vec3 screenTopRight;
    vec3 screenBottomLeft;
    vec3 screenBottomRight;
    float dy;
    float dx;

    Camera(const vec3& e, const vec3& c, const vec3& u) : eye(e), center(c), up(u) {}

    void lookAt(){
        this->w = (eye - center).unit();
        this->u = up.cross(w).unit();
        this->v = w.cross(u).unit();
        this->rotationMatrixInv = matrix3(u, v, w);
        this->rotationMatrix = matrix3(u, v, w).T();
    };

    void setScreen(float ratio, float fov, float focalLength){
        this->ratio = ratio;
        this->focalLength = focalLength;
        float halfHeight = tan(fov/2);
        this->dy = focalLength * halfHeight;
        this->dx = dy * ratio;
    };

    Ray generateRay(float nx, float ny){

        vec3 p = vec3(-dx + 2*dx*nx, -dy + 2*dy*ny, -focalLength);
        vec3 o = rotationMatrix * vec3(0,0,0);
        vec3 d = rotationMatrix * p;
        // cout << (d-o) << endl;
        return Ray(center, (o-d).unit());
        // nx++;
        // ny++;
        // float x = (nx + 0.5) /nx;
        // float y = (ny + 0.5) /ny;
        // vec3 screenPoint = screenTopLeft + (screenTopRight - screenTopLeft) * x + (screenBottomLeft - screenTopLeft) * y;
        // vec3 direction = rotationMatrix * screenPoint;
        // // cout << direction << endl;
        // return Ray(eye, direction);
    }
};

ostream& operator<<(ostream& os, const Camera& c)
{
    os << "Cam(" << c.eye << ", " << c.center << ", " << c.up << ")";
    return os;
}
