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
        Vector3<float> *amb_col, Vector3<float> *diff_col, Vector3<float> *spec_col,
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

    cout << "delete SHAPE" << endl;
}

/// Type getter
/// \return The type of the shape
string Shape::getType() { return type; }

/// Ambient colour getter
/// \return Ambient colour of the shape
Vector3<float> *Shape::get_ambient_colour() { return ambient_colour; }
/// Diffuse colour getter
/// \return Diffuse colour of the shape
Vector3<float> *Shape::get_diffuse_colour() { return diffuse_colour; }
/// Specular colour getter
/// \return Specular colour of the shape
Vector3<float> *Shape::get_specular_colour() { return specular_colour; }

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
Triangle::Triangle(Vector3<float> *p1, Vector3<float> *p2, Vector3<float> *p3,
            Vector3<float> *amb_col, Vector3<float> *diff_col, Vector3<float> *spec_col,
            float amb_coe, float diff_coe, float spec_coe,
            float phong_coe,
            Vector3<float>* normal_override, bool copy_vectors) :
        Shape("Triangle",
            copy_vectors ? new Vector3<float>(*amb_col) : amb_col,
            copy_vectors ? new Vector3<float>(*diff_col) : diff_col,
            copy_vectors ? new Vector3<float>(*spec_col) : spec_col,
            amb_coe, diff_coe, spec_coe, phong_coe) {
    if (normal_override == NULL) {
        // Checking to see where the hypotenuse is
        float p1p2 = (*p1 - *p2).norm();
        float p2p3 = (*p2 - *p3).norm();
        float p3p1 = (*p3 - *p1).norm();

        // Creating the second triangle along the hypotenuse
        if (p1p2 >= p2p3 && p1p2 >= p3p1) {
            a = copy_vectors ? new Vector3<float>(*p3) : p3;
            b = copy_vectors ? new Vector3<float>(*p1) : p2;
            c = copy_vectors ? new Vector3<float>(*p2) : p2;
        }
        else if (p2p3 >= p1p2 && p2p3 >= p3p1) {
            a = copy_vectors ? new Vector3<float>(*p1) : p1;
            b = copy_vectors ? new Vector3<float>(*p2) : p2;
            c = copy_vectors ? new Vector3<float>(*p3) : p3;
        }
        else if (p3p1 >= p1p2 && p3p1 >= p2p3) {
            a = copy_vectors ? new Vector3<float>(*p2) : p2;
            b = copy_vectors ? new Vector3<float>(*p1) : p1;
            c = copy_vectors ? new Vector3<float>(*p3) : p3;
        }
    }
    else {
        a = copy_vectors ? new Vector3<float>(*p1) : p1;
        b = copy_vectors ? new Vector3<float>(*p2) : p2;
        c = copy_vectors ? new Vector3<float>(*p3) : p3;
    }

    // Making sure both Triangles have the same normal
    normal = (normal_override == NULL)
        ? new Vector3<float>((*b - *a).cross(*c - *a).normalized())
        : new Vector3<float>(*normal_override);
}

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

    cout << "delete TRIANGLE" << endl;
}

/// Triangle stream insertion operator
/// \param strm Incoming stream
/// \param t Triangle to insert
/// \return Triangle information added to the stream
ostream& operator<<(ostream &strm, const Triangle &t) {
    return strm << "TRIANGLE" << endl
                << "\tA: " << t.a->x() << " " << t.a->y() << " " << t.a->z() << " " << endl
                << "\tB: " << t.b->x() << " " << t.b->y() << " " << t.b->z() << " " << endl
                << "\tC: " << t.c->x() << " " << t.c->y() << " " << t.c->z();
}

/// First point getter
/// \return First point
Vector3<float> *Triangle::A() { return a; }
/// Second point getter
/// \return Second point
Vector3<float> *Triangle::B() { return b; }
/// Third point getter
/// \return Third point
Vector3<float> *Triangle::C() { return c; }
/// Normal getter
/// \return Normal to the plane
Vector3<float> *Triangle::get_normal() { return normal; }



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
Rectangle::Rectangle(Vector3<float> *p1, Vector3<float> *p2, Vector3<float> *p3, Vector3<float> *p4,
        Vector3<float> *amb_col, Vector3<float> *diff_col, Vector3<float> *spec_col,
        float amb_coe, float diff_coe, float spec_coe,
        float phong_coe) :
    Shape("Rectangle", amb_col, diff_col, spec_col, amb_coe, diff_coe, spec_coe, phong_coe),
    a(p1), b(p2), c(p3), d(p4) {
    t1 = new Triangle(a, b, c, amb_col, diff_col, spec_col, amb_coe, diff_coe, spec_coe, phong_coe, NULL, true);
    t2 = new Triangle(d, t1->B(), t1->C(), amb_col, diff_col, spec_col, amb_coe, diff_coe, spec_coe, phong_coe, t1->get_normal(), true);
    normal = new Vector3<float>(*t1->get_normal());
}

/// Rectangle destructor
Rectangle::~Rectangle() {
    delete t1;
    t1 = NULL;

    delete t2;
    t2 = NULL;

    delete normal;
    normal = NULL;

    cout << "delete RECTANGLE" << endl;
}

/// Rectangle stream insertion operator
/// \param strm Incoming stream
/// \param r Rectangle to insert
/// \return Rectangle information added to the stream
ostream& operator<<(ostream &strm, const Rectangle &r) {
    return strm << "RECTANGLE" << endl
                << "\tA: " << r.t1->A()->x() << " " << r.t1->A()->y() << " " << r.t1->A()->z() << " " << endl
                << "\tB: " << r.t1->B()->x() << " " << r.t1->B()->y() << " " << r.t1->B()->z() << " " << endl
                << "\tC: " << r.t1->C()->x() << " " << r.t1->C()->y() << " " << r.t1->C()->z() << " " << endl
                << "\tD: " << r.t2->C()->x() << " " << r.t2->C()->y() << " " << r.t2->C()->z() << " " << endl;
}

/// Triangle 1 getter
/// \return Triangle 1
Triangle *Rectangle::getT1() { return t1; }
/// Triangle 2 getter
/// \return Triangle 2
Triangle *Rectangle::getT2() { return t2; }

/// First point getter
/// \return First point
Vector3<float> *Rectangle::A() { return a; }
/// Second point getter
/// \return Second point
Vector3<float> *Rectangle::B() { return b; }
/// Third point getter
/// \return Third point
Vector3<float> *Rectangle::C() { return c; }
/// Fourth point getter
/// \return Fourth point
Vector3<float> *Rectangle::D() { return d; }
/// Normal getter
/// \return Normal to the plane
Vector3<float> *Rectangle::get_normal() { return t1->get_normal(); }



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
Shape3D::Shape3D(string t, Vector3<float> *o,
        Vector3<float> *amb_col, Vector3<float> *diff_col, Vector3<float> *spec_col,
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
Vector3<float> *Shape3D::get_origin() { return origin; }



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
Sphere::Sphere(Vector3<float> *o, float r,
        Vector3<float> *amb_col, Vector3<float> *diff_col, Vector3<float> *spec_col,
        float amb_coe, float diff_coe, float spec_coe,
        float phong_coe) :
    Shape3D("Sphere", o, amb_col, diff_col, spec_col, amb_coe, diff_coe, spec_coe, phong_coe), radius(r) { }

/// Sphere destructor
Sphere::~Sphere() {
    cout << "delete SPHERE" << endl;
}

/// Sphere stream insertion operator
/// \param strm Incoming stream
/// \param s Sphere to insert
/// \return Sphere information added to the stream
ostream& operator<<(ostream &strm, const Sphere &s) {
    return strm << "SPHERE" << endl
                << "\tOrigin: " << s.origin->x() << " " << s.origin->y() << " " << s.origin->z() << endl
                << "\tRadius: " << s.radius;
}

/// Radius getter
/// \return Radius of the sphere
float Sphere::get_radius() const { return radius; }