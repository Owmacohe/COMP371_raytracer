#include "Shapes.h"

/* ### SHAPE ### */

/// Shape abstract class constructor
/// \param t The type of the shape
/// \param amb_col Ambient colour of the shape
/// \param diff_col Diffuse colour of the shape
/// \param spec_col Specular colour of the shape
/// \param amb_coe Ambient coefficient of the shape
/// \param diff_coe Diffuse coefficient of the shape
/// \param spec_coe Specular coefficient of the shape
/// \param phong_coe Phone coefficient of the shape
Shape::Shape(string t,
        Vector3f *amb_col, Vector3f *diff_col, Vector3f *spec_col,
        float amb_coe, float diff_coe, float spec_coe,
        float phong_coe) :
    type(t),
    ambient_colour(amb_col), diffuse_colour(diff_col), specular_colour(spec_col),
    ambient_coefficient(amb_coe), diffuse_coefficient(diff_coe), specular_coefficient(spec_coe),
    phong_coefficient(phong_coe) { }

/// Shape destructor
Shape::~Shape() {
    delete ambient_colour;
    ambient_colour = NULL;

    delete diffuse_colour;
    diffuse_colour = NULL;

    delete specular_colour;
    specular_colour = NULL;

    //cout << "delete SHAPE" << endl;
}

/// Type getter
/// \return The type of the shape
string Shape::get_type() { return type; }

/// Ambient colour getter
/// \return Ambient colour of the shape
Vector3f *Shape::get_ambient_colour() { return ambient_colour; }
/// Diffuse colour getter
/// \return Diffuse colour of the shape
Vector3f *Shape::get_diffuse_colour() { return diffuse_colour; }
/// Specular colour getter
/// \return Specular colour of the shape
Vector3f *Shape::get_specular_colour() { return specular_colour; }

/// Ambient coefficient getter
/// \return Ambient coefficient of the shape
float Shape::get_ambient_coefficient() { return ambient_coefficient; }
/// Diffuse coefficient getter
/// \return Diffuse coefficient of the shape
float Shape::get_diffuse_coefficient() { return diffuse_coefficient; }
/// Specular coefficient getter
/// \return Specular coefficient of the shape
float Shape::get_specular_coefficient() { return specular_coefficient; }

/// Phong coefficient getter
/// \return Phone coefficient of the shape
float Shape::get_phong_coefficient() { return phong_coefficient; }



/* ### Triangle ### */

/// Triangle class constructor
/// \param p1 First point
/// \param p2 Second point
/// \param p3 Third point
/// \param amb_col Ambient colour of the shape
/// \param diff_col Diffuse colour of the shape
/// \param spec_col Specular colour of the shape
/// \param amb_coe Ambient coefficient of the shape
/// \param diff_coe Diffuse coefficient of the shape
/// \param spec_coe Specular coefficient of the shape
/// \param phong_coe Phone coefficient of the shape
/// \param n Normal for the triangle
Triangle::Triangle(Vector3f *p1, Vector3f *p2, Vector3f *p3,
        Vector3f *amb_col, Vector3f *diff_col, Vector3f *spec_col,
        float amb_coe, float diff_coe, float spec_coe,
        float phong_coe,
        Vector3f *n) :
    Shape("Triangle",
        new Vector3f(*amb_col),
        new Vector3f(*diff_col),
        new Vector3f(*spec_col),
        amb_coe, diff_coe, spec_coe, phong_coe),
    a(new Vector3f(*p1)),
    b(new Vector3f(*p2)),
    c(new Vector3f(*p3)),
    normal(new Vector3f(*n)) { }

/// Triangle destructor
Triangle::~Triangle() {
    delete a;
    a = NULL;

    delete b;
    b = NULL;

    delete c;
    c = NULL;

    delete normal;
    normal = NULL;

    //cout << "delete TRIANGLE" << endl;
}

/// First point getter
/// \return First point
Vector3f *Triangle::A() { return a; }
/// Second point getter
/// \return Second point
Vector3f *Triangle::B() { return b; }
/// Third point getter
/// \return Third point
Vector3f *Triangle::C() { return c; }
/// Normal getter
/// \return Normal to the plane
Vector3f *Triangle::get_normal() { return normal; }



/* ### Rectangle ### */

/// Rectangle class constructor
/// \param p1 First point
/// \param p2 Second point
/// \param p3 Third point
/// \param p4 Fourth point
/// \param amb_col Ambient colour of the shape
/// \param diff_col Diffuse colour of the shape
/// \param spec_col Specular colour of the shape
/// \param amb_coe Ambient coefficient of the shape
/// \param diff_coe Diffuse coefficient of the shape
/// \param spec_coe Specular coefficient of the shape
/// \param phong_coe Phone coefficient of the shape
Rectangle::Rectangle(Vector3f *p1, Vector3f *p2, Vector3f *p3, Vector3f *p4,
        Vector3f *amb_col, Vector3f *diff_col, Vector3f *spec_col,
        float amb_coe, float diff_coe, float spec_coe,
        float phong_coe) :
    Shape("Rectangle", amb_col, diff_col, spec_col, amb_coe, diff_coe, spec_coe, phong_coe),
    a(p1), b(p2), c(p3), d(p4) {
    // Calculating the normal for this Rectangle, then setting two new Triangles with the normal
    normal = new Vector3f(-(*b - *a).cross(*c - *a).normalized());
    // t2 has to use points { a, c, d } to work for the JSONs
    t1 = new Triangle(a, b, c, amb_col, diff_col, spec_col, amb_coe, diff_coe, spec_coe, phong_coe, normal);
    t2 = new Triangle(a, c, d, amb_col, diff_col, spec_col, amb_coe, diff_coe, spec_coe, phong_coe, normal);
}

/// Rectangle destructor
Rectangle::~Rectangle() {
    delete t1;
    t1 = NULL;

    delete t2;
    t2 = NULL;

    delete normal;
    normal = NULL;

    //cout << "delete RECTANGLE" << endl;
}

/// Triangle 1 getter
/// \return Triangle 1
Triangle *Rectangle::getT1() { return t1; }
/// Triangle 2 getter
/// \return Triangle 2
Triangle *Rectangle::getT2() { return t2; }

/// First point getter
/// \return First point
Vector3f *Rectangle::A() { return a; }
/// Second point getter
/// \return Second point
Vector3f *Rectangle::B() { return b; }
/// Third point getter
/// \return Third point
Vector3f *Rectangle::C() { return c; }
/// Fourth point getter
/// \return Fourth point
Vector3f *Rectangle::D() { return d; }
/// Normal getter
/// \return Normal to the plane
Vector3f *Rectangle::get_normal() { return t1->get_normal(); }



/* ### Shape3D ### */

/// Shape3D abstract class constructor
/// \param t The type of the shape
/// \param o The origin of the shape
/// \param amb_col Ambient colour of the shape
/// \param diff_col Diffuse colour of the shape
/// \param spec_col Specular colour of the shape
/// \param amb_coe Ambient coefficient of the shape
/// \param diff_coe Diffuse coefficient of the shape
/// \param spec_coe Specular coefficient of the shape
/// \param phong_coe Phone coefficient of the shape
Shape3D::Shape3D(string t, Vector3f *o,
        Vector3f *amb_col, Vector3f *diff_col, Vector3f *spec_col,
        float amb_coe, float diff_coe, float spec_coe,
        float phong_coe) :
    Shape(t, amb_col, diff_col, spec_col, amb_coe, diff_coe, spec_coe, phong_coe), origin(o) { }

/// Shape3D destructor
Shape3D::~Shape3D() {
    delete origin;
    origin = NULL;
}

/// Origin getter
/// \return The origin of the shape
Vector3f *Shape3D::get_origin() { return origin; }



/* ### Sphere ### */

/// Sphere class constructor
/// \param o The origin of the shape
/// \param r Radius of the sphere
/// \param amb_col Ambient colour of the shape
/// \param diff_col Diffuse colour of the shape
/// \param spec_col Specular colour of the shape
/// \param amb_coe Ambient coefficient of the shape
/// \param diff_coe Diffuse coefficient of the shape
/// \param spec_coe Specular coefficient of the shape
/// \param phong_coe Phone coefficient of the shape
Sphere::Sphere(Vector3f *o, float r,
        Vector3f *amb_col, Vector3f *diff_col, Vector3f *spec_col,
        float amb_coe, float diff_coe, float spec_coe,
        float phong_coe) :
    Shape3D("Sphere", o, amb_col, diff_col, spec_col, amb_coe, diff_coe, spec_coe, phong_coe), radius(r) { }

/// Sphere destructor
Sphere::~Sphere() {
    //cout << "delete SPHERE" << endl;
}

/// Radius getter
/// \return Radius of the sphere
float Sphere::get_radius() const { return radius; }