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
using Eigen::Vector2;
using Eigen::Vector3;

class Shape;
class Triangle;
class Sphere;

class Camera;
class Image;
class Light;
class Area;
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
    vector<Output*> parse_outputs();

    vector<Shape*> shapes;
    vector<Light*> lights;
    vector<Output*> outputs;
};

Vector3<float> get_triangle_raycast(Vector3<float>*, Vector3<float>*, Vector3<float>);
bool is_on_right(Vector3<float>, Vector3<float>, Vector3<float>, Vector3<float>);
float clamp(float, float);
float clamp(float, float, float);

class Ray {
public:
    Ray(Image*, Camera*, Vector3<float>, Shape*, Vector2<float>, Vector2<float>);
    ~Ray();

    Vector3<float> get_intensity(Vector3<float>*, Shape*, Vector3<float>, float, vector<Shape*>);

    Vector3<float> *get_raycast();
    Vector3<float> *get_hit();
    bool get_does_hit();
private:
    bool hit_triangle(Vector3<float>, Triangle*, bool), hit_sphere(Vector3<float>, Vector3<float>, Sphere*, bool);

    Vector3<float> *raycast;
    Vector3<float> *hit;
    bool does_hit;
};