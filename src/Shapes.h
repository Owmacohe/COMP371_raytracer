#pragma once

#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

using namespace std;
using Eigen::Vector3;

class Shape {
public:
    Shape(string, Vector3<float>*, Vector3<float>*, Vector3<float>*, float, float, float, float);
    ~Shape();

    virtual string getType();
    virtual Vector3<float> *get_ambient_colour(), *get_diffuse_colour(), *get_specular_colour();
    virtual float get_ambient_coefficient(), get_diffuse_coefficient(), get_specular_coefficient();
    virtual float get_phong_coefficient();
protected:
    string type;
    Vector3<float> *ambient_colour, *diffuse_colour, *specular_colour;
    float ambient_coefficient, diffuse_coefficient, specular_coefficient;
    float phong_coefficient;
};

class Triangle : public Shape {
public:
    Triangle(Vector3<float>*, Vector3<float>*, Vector3<float>*,
        Vector3<float>*, Vector3<float>*, Vector3<float>*,
        float, float, float,
        float, Vector3<float>*, bool);
    ~Triangle();

    friend ostream& operator<<(ostream &strm, const Triangle &t);

    Vector3<float> *A(), *B(), *C(), *get_normal();
private:
    Vector3<float> *a, *b, *c, *normal;
    bool is_second_triangle;
};

class Rectangle : public Shape {
public:
    Rectangle(Vector3<float>*, Vector3<float>*, Vector3<float>*, Vector3<float>*,
        Vector3<float>*, Vector3<float>*, Vector3<float>*,
        float, float, float,
        float);
    ~Rectangle();

    friend ostream& operator<<(ostream &strm, const Rectangle &p);

    Triangle *getT1(), *getT2();
    Vector3<float> *A(), *B(), *C(), *D(), *get_normal();
private:
    Triangle *t1, *t2;
};

class Shape3D : public Shape {
public:
    Shape3D(string, Vector3<float>*, Vector3<float>*, Vector3<float>*, Vector3<float>*, float, float, float, float);
    ~Shape3D();

    Vector3<float> *get_origin();
protected:
    Vector3<float> *origin;
};

class Sphere : public Shape3D {
public:
    Sphere(Vector3<float>*, float, Vector3<float>*, Vector3<float>*, Vector3<float>*, float, float, float, float);
    ~Sphere();

    friend ostream& operator<<(ostream &strm, const Sphere &s);

    float get_radius() const;
private:
    float radius;
};