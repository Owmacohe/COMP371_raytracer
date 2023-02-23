#pragma once

#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>

#include <Eigen/Core>
#include <Eigen/Dense>

#include "../external/json.hpp"
#include "../external/simpleppm.h"

using namespace std;
using namespace nlohmann;
using Eigen::Vector3;

class Shape;
class Triangle;
class Sphere;

class Light;
class Output;

class RayTracer {
public:
    RayTracer(json);
    ~RayTracer();

    void run();
private:
    json *j;

    vector<Shape*> parse_geometry();
    vector<Light*> parse_lights();
    Output *parse_output();

    vector<Shape*> shapes;
    vector<Light*> lights;
    Output *output;
};

class Camera {
public:
    Camera(Vector3<float>*, Vector3<float>*, Vector3<float>*);
    ~Camera();

    friend ostream& operator<<(ostream &strm, const Camera &c);

    Vector3<float> *get_origin(), *get_look_at(), *get_up(), *get_side();
private:
    Vector3<float> *origin, *look_at, *up, *side;
};

class Image {
public:
    Image(string, int, int, float, Vector3<float>*, Vector3<float>*);
    ~Image();

    friend ostream& operator<<(ostream &strm, const Image &i);

    string get_name();
    int get_width() const, get_height() const;
    float get_FOV() const, get_alpha() const, get_pixel_size() const;
    int get_number_of_hits();

    Vector3<float> *get_ambient(), *get_background();

    void raycast(Camera*, Shape*, bool, bool, bool);
private:
    vector<double> *buffer;

    string name;
    int width, height;
    float FOV;
    int number_of_hits;

    Vector3<float> *ambient;
    Vector3<float> *background;
};

bool is_on_right(Vector3<float>*, Vector3<float>*, Vector3<float>*, Vector3<float>*);

class Ray {
public:
    Ray(Image*, Camera*, Vector3<float>,Shape*, int, int);
    ~Ray();

    Vector3<float> get_intensity();

    Vector3<float> *get_raycast();
    Vector3<float> *get_hit();
    bool get_does_hit();
private:
    bool hit_triangle(Triangle*), hit_sphere(Vector3<float>*, Sphere*);

    Vector3<float> *raycast;
    Vector3<float> *hit;
    bool does_hit;
};