#pragma once

#include <random>

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
    int get_n();
    bool get_usecenter();
private:
    Vector3f *p1, *p2, *p3, *p4;
    int n;
    bool usecenter;
};

class Camera {
public:
    Camera(Vector3f*, Vector3f*, Vector3f*, float);
    ~Camera();

    Vector3f *get_origin(), *get_look_at(), *get_up(), *get_side();
    float get_FOV() const;
private:
    Vector3f *origin, *look_at, *up, *side;
    float FOV;
};

bool check_probterminate(float);
Vector3f get_new_bounce_direction(Vector3f, Vector3f);
float get_cos_angle(Vector3f, Vector3f);

class Image {
public:
    Image(string, int, int, Vector3f*, Vector3f*, vector<int>, bool, bool, int, float);
    ~Image();

    string get_name();
    int get_width() const, get_height() const;
    float get_alpha(Camera*) const, get_pixel_size(Camera*) const;

    Vector3f *get_ambient(), *get_background();

    vector<int> get_raysperpixel();
    bool get_antialiasing(), get_globalillum();

    Vector3f get_base(Camera*);
    void raycast(Camera*, Image*, Shape*, vector<Light*>, vector<Shape*>);
private:
    void local_raycast(Camera*, Image*, Shape*, vector<Light*>, vector<Shape*>);
    void global_raycast(Camera*, Image*, Shape*, vector<Light*>, vector<Shape*>);

    vector<double> *buffer;

    string name;
    int width, height;

    Vector3f *ambient;
    Vector3f *background;

    vector<int> raysperpixel;
    bool antialiasing, globalillum;
    int maxbounces;
    float probterminate;
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