#pragma once

#include <random>

#include "RayTracer.h"

class Camera {
public:
    Camera(Vector3f*, Vector3f*, Vector3f*, float);
    ~Camera();

    friend ostream& operator<<(ostream &strm, const Camera &c);

    Vector3f *get_origin(), *get_look_at(), *get_up(), *get_side();
    float get_FOV() const;
private:
    Vector3f *origin, *look_at, *up, *side;
    float FOV;
};

class Image {
public:
    Image(string, int, int, Vector3f*, Vector3f*, vector<int>, bool, bool, int, float);
    ~Image();

    friend ostream& operator<<(ostream &strm, const Image &i);

    string get_name();
    int get_width() const, get_height() const;
    float get_alpha(Camera*) const, get_pixel_size(Camera*) const;
    int get_number_of_hits();

    Vector3f *get_ambient(), *get_background();

    vector<int> get_raysperpixel();
    bool get_antialiasing(), get_globalillum();

    void raycast(Camera*, Image*, Shape*, vector<Light*>, bool, bool, vector<Shape*>);
private:
    void local_raycast(Camera*, Image*, Shape*, vector<Light*>, bool, bool, vector<Shape*>);

    Vector2f get_stratified_offset(float, float);
    Vector3f get_initial_outgoing_global_ray(Camera*, Vector2f, Vector3f, Vector2f, float);
    bool check_probterminate();
    void global_raycast(Camera*, Image*, Shape*, vector<Light*>, bool, bool, vector<Shape*>);

    vector<double> *buffer;

    string name;
    int width, height;
    int number_of_hits;

    Vector3f *ambient;
    Vector3f *background;

    vector<int> raysperpixel;
    bool antialiasing, globalillum;
    int maxbounces;
    float probterminate;
};

class Light {
public:
    Light(string, Vector3f*, Vector3f*);
    ~Light();

    virtual string get_type();
    virtual Vector3f *get_diffuse_intensity(), *get_specular_intensity();
protected:
    string type;
    Vector3f *diffuse_intensity, *specular_intensity;
};

class Point : public Light {
public:
    Point(Vector3f*, Vector3f*, Vector3f*);
    ~Point();

    Vector3f *get_origin();
private:
    Vector3f *origin;
};

class Area : public Light {
public:
    Area(Vector3f*, Vector3f*, Vector3f*, Vector3f*, Vector3f*, Vector3f*, int, bool);
    ~Area();

    Vector3f *P1(), *P2(), *P3(), *P4();
private:
    Vector3f *p1, *p2, *p3, *p4;
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