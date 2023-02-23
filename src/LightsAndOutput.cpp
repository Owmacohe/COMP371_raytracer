#include "LightsAndOutput.h"

/* ### Light ### */

/// Light class constructor
/// \param t Type of the light
/// \param diff_int Diffuse intensity of the light
/// \param spec_int Specular intensity of the light
Light::Light(string t, Vector3<float> *diff_int, Vector3<float> *spec_int) :
    type(t), diffuse_intensity(diff_int), specular_intensity(spec_int) { }

Light::~Light() {
    delete diffuse_intensity;
    diffuse_intensity = NULL;

    delete specular_intensity;
    specular_intensity = NULL;

    cout << "delete LIGHT" << endl;
}

/// Type getter
/// \return Type of the light
string Light::get_type() { return type; }

/// Diffuse intensity getter
/// \return Diffuse intensity of the light
Vector3<float> *Light::get_diffuse_intensity() { return diffuse_intensity; }
/// Specular intensity getter
/// \return Specular intensity of the light
Vector3<float> *Light::get_specular_intensity() { return specular_intensity; }



/* ### Point ### */

/// Point light class constructor
/// \param o Origin of the light
/// \param diff_int Diffuse intensity of the light
/// \param spec_int Specular intensity of the light
Point::Point(Vector3<float> *o, Vector3<float> *diff_int, Vector3<float> *spec_int) :
    Light("Point", diff_int, spec_int), origin(o) { }

/// Point light destructor
Point::~Point() {
    delete origin;
    origin = NULL;

    cout << "delete POINT LIGHT" << endl;
}

/// Origin getter
/// \return Origin of the light
Vector3<float> *Point::get_origin() { return origin; }



/* ### Area ### */

/// Area light class constructor
/// \param a First point
/// \param b Second point
/// \param c Third point
/// \param d Fourth point
/// \param diff_int Diffuse intensity of the light
/// \param spec_int Specular intensity of the light
Area::Area(
        Vector3<float>* a, Vector3<float>* b, Vector3<float>* c, Vector3<float>* d,
        Vector3<float> *diff_int, Vector3<float> *spec_int) :
    Light("Area", diff_int, spec_int),
    p1(a), p2(b), p3(c), p4(d) { }

/// Area light constructor
Area::~Area() {
    delete p1;
    p1 = NULL;

    delete p2;
    p2 = NULL;

    delete p3;
    p3 = NULL;

    delete p4;
    p4 = NULL;
}

/// First point getter
/// \return First point
Vector3<float> *Area::P1() { return p1; }
/// Second point getter
/// \return Second point
Vector3<float> *Area::P2() { return p2; }
/// Third point getter
/// \return Third point
Vector3<float> *Area::P3() { return p3; }
/// Fourth point getter
/// \return Fourth point
Vector3<float> *Area::P4() { return p4; }



/* ### Output ### */

/// Output container class constructor
/// \param img Scene image
/// \param cam Scene camera
Output::Output(Image *img, Camera *cam) : image(img), camera(cam) { }

/// Output destructor
Output::~Output() {
    delete image;
    image = NULL;

    delete camera;
    camera = NULL;

    cout << "delete OUTPUT" << endl;
}

/// Scene image getter
/// \return Scene image
Image *Output::get_image() { return image; }
/// Scene camera getter
/// \return Scene camera
Camera *Output::get_camera() { return camera; }