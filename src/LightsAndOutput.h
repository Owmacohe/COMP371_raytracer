#pragma once

#include <random>

#include "RayTracer.h"

class Camera {
public:
    Camera(Vector3<float>*, Vector3<float>*, Vector3<float>*, float);
    ~Camera();

    friend ostream& operator<<(ostream &strm, const Camera &c);

    Vector3<float> *get_origin(), *get_look_at(), *get_up(), *get_side();
    float get_FOV() const;
private:
    Vector3<float> *origin, *look_at, *up, *side;
    float FOV;
};

class Image {
public:
    Image(string, int, int, Vector3<float>*, Vector3<float>*, vector<int>, bool, bool);
    ~Image();

    friend ostream& operator<<(ostream &strm, const Image &i);

    string get_name();
    int get_width() const, get_height() const;
    float get_alpha(Camera*) const, get_pixel_size(Camera*) const;
    int get_number_of_hits();

    Vector3<float> *get_ambient(), *get_background();

    vector<int> get_raysperpixel();
    bool get_antialiasing(), get_globalillum();

    void raycast(Camera*, Shape*, vector<Light*>, bool, bool, bool, vector<Shape*>);
private:
    vector<double> *buffer;

    string name;
    int width, height;
    int number_of_hits;

    Vector3<float> *ambient;
    Vector3<float> *background;

    vector<int> raysperpixel;
    bool antialiasing, globalillum;
};

class Light {
public:
    Light(string, Vector3<float>*, Vector3<float>*);
    ~Light();

    virtual string get_type();
    virtual Vector3<float> *get_diffuse_intensity(), *get_specular_intensity();
protected:
    string type;
    Vector3<float> *diffuse_intensity, *specular_intensity;
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
    Area(Vector3<float>*, Vector3<float>*, Vector3<float>*, Vector3<float>*, Vector3<float>*, Vector3<float>*, int, bool);
    ~Area();

    Vector3<float> *P1(), *P2(), *P3(), *P4();
private:
    Vector3<float> *p1, *p2, *p3, *p4;
    int n;
    bool usecenter;
};

class Output {
public:
    Output(Camera*, Image*);
    ~Output();

    Camera *get_camera();
    Image *get_image();
private:
    Camera *camera;
    Image *image;
};