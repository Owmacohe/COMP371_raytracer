#pragma once

#include "RayTracer.h"

class Image;
class Camera;

class Light {
public:
    Light(string, Vector3<float>*, Vector3<float>*);
    ~Light();

    virtual string getType();
    virtual Vector3<float> *getDiffuseIntensity(), *getSpecularIntensity();
protected:
    string type;
    Vector3<float> *diffuseIntensity, *specularIntensity;
};

class Point : public Light {
public:
    Point(Vector3<float>*, Vector3<float>*, Vector3<float>*);
    ~Point();

    Vector3<float> *getOrigin();
private:
    Vector3<float> *origin;
};

class Area : public Light {
public:
    Area(Vector3<float>*, Vector3<float>*, Vector3<float>*, Vector3<float>*, Vector3<float>*, Vector3<float>*);
    ~Area();

    Vector3<float> *getP1(), *getP2(), *getP3(), *getP4();
private:
    Vector3<float> *p1, *p2, *p3, *p4;
};

class Output {
public:
    Output(Image*, Camera*);
    ~Output();

    Image *getImage();
    Camera *getCamera();
private:
    Image *image;
    Camera *camera;
};