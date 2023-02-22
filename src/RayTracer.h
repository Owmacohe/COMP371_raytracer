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
    Camera(Vector3<float>*, Vector3<float>*, Vector3<float>*, float);
    ~Camera();

    friend ostream& operator<<(ostream &strm, const Camera &c);

    Vector3<float> *getOrigin(), *getLookAt(), *getUp(), *getSide();
    float getFocalLength();
private:
    Vector3<float> *origin, *lookAt, *up, *side;
    float focalLength;
};

class Image {
public:
    Image(string, float, float, float, Vector3<float>*, Vector3<float>*);
    ~Image();

    friend ostream& operator<<(ostream &strm, const Image &i);

    string getName();
    float getWidth() const, getHeight() const, getFOV() const, getDelta() const;
    int getNumberOfHits();

    Vector3<float> *getAmbient(), *getBackground();

    void raycast(Camera*, Shape*, bool, bool, bool);
private:
    vector<double> *buffer;

    string name;
    float width, height, FOV;
    int numberOfHits;

    Vector3<float> *ambient;
    Vector3<float> *background;
};

bool isOnRight(Vector3<float>*, Vector3<float>*, Vector3<float>*, Vector3<float>*);

class Ray {
public:
    Ray(Image*, Camera*, Shape*, int, int);
    ~Ray();

    Vector3<float> getIntensity();

    Vector3<float> *getRaycast();
    Vector3<float> *getHit();
    bool getDoesHit();
private:
    bool hitTriangle(Triangle*), hitSphere(Vector3<float>, Sphere*);

    Vector3<float> *raycast;
    Vector3<float> *hit;
    bool doesHit;
};