#pragma once

#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

using namespace std;
using Eigen::Vector3f;

class Shape {
public:
    Shape(string, Vector3f*, Vector3f*, Vector3f*, float, float, float, float);
    ~Shape();

    virtual string getType();
    virtual Vector3f *get_ambient_colour(), *get_diffuse_colour(), *get_specular_colour();
    virtual float get_ambient_coefficient(), get_diffuse_coefficient(), get_specular_coefficient();
    virtual float get_phong_coefficient();
protected:
    string type;
    Vector3f *ambient_colour, *diffuse_colour, *specular_colour;
    float ambient_coefficient, diffuse_coefficient, specular_coefficient;
    float phong_coefficient;
};

class Triangle : public Shape {
public:
    Triangle(Vector3f*, Vector3f*, Vector3f*,
        Vector3f*, Vector3f*, Vector3f*,
        float, float, float,
        float,
        Vector3f*);
    ~Triangle();

    friend ostream& operator<<(ostream &strm, const Triangle &t);

    Vector3f *A(), *B(), *C(), *get_normal();
private:
    Vector3f *a, *b, *c, *normal;
};

class Rectangle : public Shape {
public:
    Rectangle(Vector3f*, Vector3f*, Vector3f*, Vector3f*,
        Vector3f*, Vector3f*, Vector3f*,
        float, float, float,
        float);
    ~Rectangle();

    friend ostream& operator<<(ostream &strm, const Rectangle &p);

    Triangle *getT1(), *getT2();
    Vector3f *A(), *B(), *C(), *D(), *get_normal();
private:
    Vector3f *a, *b, *c, *d, *normal;
    Triangle *t1, *t2;
};

class Shape3D : public Shape {
public:
    Shape3D(string, Vector3f*, Vector3f*, Vector3f*, Vector3f*, float, float, float, float);
    ~Shape3D();

    Vector3f *get_origin();
protected:
    Vector3f *origin;
};

class Sphere : public Shape3D {
public:
    Sphere(Vector3f*, float, Vector3f*, Vector3f*, Vector3f*, float, float, float, float);
    ~Sphere();

    friend ostream& operator<<(ostream &strm, const Sphere &s);

    float get_radius() const;
private:
    float radius;
};