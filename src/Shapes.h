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
    virtual Vector3<float> *getAmbientColour(), *getDiffuseColour(), *getSpecularColour();
    virtual float getAmbientCoefficient(), getDiffuseCoefficient(), getSpecularCoefficient();
    virtual float getPhongCoefficient();
protected:
    string type;
    Vector3<float> *ambientColour, *diffuseColour, *specularColour;
    float ambientCoefficient, diffuseCoefficient, specularCoefficient;
    float phongCoefficient;
};

class Triangle : public Shape {
public:
    Triangle(Vector3<float>*, Vector3<float>*, Vector3<float>*,
        Vector3<float>*, Vector3<float>*, Vector3<float>*,
        float, float, float,
        float, bool);
    ~Triangle();

    friend ostream& operator<<(ostream &strm, const Triangle &t);

    Vector3<float> *A(), *B(), *C(), *getNormal();
private:
    Vector3<float> *a, *b, *c, *normal;
    bool isSecondTriangle;
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
    Vector3<float> *A(), *B(), *C(), *D(), *getNormal();
private:
    Triangle *t1, *t2;
};

class Shape3D : public Shape {
public:
    Shape3D(string, Vector3<float>*, Vector3<float>*, Vector3<float>*, Vector3<float>*, float, float, float, float);
    ~Shape3D();

    Vector3<float> *getOrigin();
protected:
    Vector3<float> *origin;
};

class Sphere : public Shape3D {
public:
    Sphere(Vector3<float>*, float, Vector3<float>*, Vector3<float>*, Vector3<float>*, float, float, float, float);
    ~Sphere();

    friend ostream& operator<<(ostream &strm, const Sphere &s);

    float getRadius();
private:
    float radius;
};