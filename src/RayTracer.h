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
using Eigen::Vector2f;
using Eigen::Vector3f;

class Shape;
class Triangle;
class Rectangle;
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

Vector3f triangle_intersect(Vector3f, Vector3f, Vector3f);
bool is_on_right(Vector3f, Vector3f, Vector3f, Vector3f);
Vector3f project_to_plane(Vector3f, Vector3f, Vector3f, Vector3f);
float clamp(float, float);
float clamp(float, float, float);

class Ray {
public:
    Ray(Vector3f, Vector3f, Shape*, bool);
    ~Ray();

    Vector3f get_intensity(Vector3f*, Shape*, Vector3f, vector<Shape*>, bool);
    Vector3f get_average_intensity(Vector3f*, Shape*, vector<Shape*>, vector<Light*>, bool);

    Vector3f *get_raycast();
    Vector3f *get_hit(), *get_hit_normal();
    Shape *get_hit_shape();
    bool get_does_hit();
private:
    bool hit_triangle(Vector3f, Triangle*);
    bool hit_rectangle(Vector3f, Vector3f, Rectangle*);
    bool hit_sphere(Vector3f, Vector3f, Sphere*);

    Vector3f *raycast;
    Vector3f *hit, *hit_normal;
    Shape *hit_shape;
    bool does_hit;
};