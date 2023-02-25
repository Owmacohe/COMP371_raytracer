#pragma once

#include "RayTracer.h"

class Image;
class Camera;

class Light {
public:
    Light(string, Vector3<float>*, Vector3<float>*);
    ~Light();

    virtual string get_type();
    virtual Vector3<float> *get_diffuse_intensity(), *get_specular_intensity();
protected:
    string type;
    Vector3<float> *diffuse_intensity, *specular_intensity; // TODO: how to use these?
};

class Point : public Light {
public:
    Point(Vector3<float>*, Vector3<float>*, Vector3<float>*);
    ~Point();

    Vector3<float> *get_origin();
private:
    Vector3<float> *origin;
};

class Area : public Light {
public:
    Area(Vector3<float>*, Vector3<float>*, Vector3<float>*, Vector3<float>*, Vector3<float>*, Vector3<float>*);
    ~Area();

    Vector3<float> *P1(), *P2(), *P3(), *P4();
private:
    Vector3<float> *p1, *p2, *p3, *p4;
};

class Output {
public:
    Output(Image*, Camera*);
    ~Output();

    Image *get_image();
    Camera *get_camera();
private:
    Image *image;
    Camera *camera;
};