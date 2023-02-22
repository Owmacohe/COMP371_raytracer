#include "Shapes.h"

/* ### SHAPE ### */

/// Shape abstract class constructor
/// \param t The type of the shape
/// \param ambCol Ambient colour of the shape
/// \param diffCol Diffuse colour of the shape
/// \param specCol Specular colour of the shape
/// \param ambCoe Ambient coefficient of the shape
/// \param diffCoe Diffuse coefficient of the shape
/// \param specCoe Specular coefficient of the shape
/// \param phongCoe Phone coefficient of the shape
Shape::Shape(string t,
        Vector3<float> *ambCol, Vector3<float> *diffCol, Vector3<float> *specCol,
        float ambCoe, float diffCoe, float specCoe,
        float phongCoe) :
    type(t),
    ambientColour(ambCol), diffuseColour(ambCol), specularColour(ambCol),
    ambientCoefficient(ambCoe), diffuseCoefficient(diffCoe), specularCoefficient(specCoe),
    phongCoefficient(phongCoe) { }

/// Shape destructor
Shape::~Shape() {
    delete ambientColour;
    ambientColour = NULL;

    delete diffuseColour;
    diffuseColour = NULL;

    delete specularColour;
    specularColour = NULL;

    cout << "delete SHAPE" << endl;
}

/// Type getter
/// \return The type of the shape
string Shape::getType() { return type; }

/// Ambient colour getter
/// \return Ambient colour of the shape
Vector3<float> *Shape::getAmbientColour() { return ambientColour; }
/// Diffuse colour getter
/// \return Diffuse colour of the shape
Vector3<float> *Shape::getDiffuseColour() { return diffuseColour; }
/// Specular colour getter
/// \return Specular colour of the shape
Vector3<float> *Shape::getSpecularColour() { return specularColour; }

/// Ambient coefficient getter
/// \return Ambient coefficient of the shape
float Shape::getAmbientCoefficient() { return ambientCoefficient; }
/// Diffuse coefficient getter
/// \return Diffuse coefficient of the shape
float Shape::getDiffuseCoefficient() { return diffuseCoefficient; }
/// Specular coefficient getter
/// \return Specular coefficient of the shape
float Shape::getSpecularCoefficient() { return specularCoefficient; }

/// Phong coefficient getter
/// \return Phone coefficient of the shape
float Shape::getPhongCoefficient() { return phongCoefficient; }



/* ### Triangle ### */

/// Triangle class constructor
/// \param p1 First point
/// \param p2 Second point
/// \param p3 Third point
/// \param ambCol Ambient colour of the shape
/// \param diffCol Diffuse colour of the shape
/// \param specCol Specular colour of the shape
/// \param ambCoe Ambient coefficient of the shape
/// \param diffCoe Diffuse coefficient of the shape
/// \param specCoe Specular coefficient of the shape
/// \param phongCoe Phone coefficient of the shape
/// \param isSecond Whether this triangle is the second of a square
Triangle::Triangle(Vector3<float> *p1, Vector3<float> *p2, Vector3<float> *p3,
            Vector3<float> *ambCol, Vector3<float> *diffCol, Vector3<float> *specCol,
            float ambCoe, float diffCoe, float specCoe,
            float phongCoe, bool isSecond) :
        Shape("Triangle", ambCol, diffCol, specCol, ambCoe, diffCoe, specCoe, phongCoe),
        a(p1), b(p2), c(p3),
        isSecondTriangle(isSecond) {
    Vector3<float> temp = (*c - *a).cross(*b - *a);
    normal = new Vector3<float>(temp / temp.norm());
}

/// Triangle destructor
Triangle::~Triangle() {
    if (!isSecondTriangle) {
        delete a;
        a = NULL;

        delete b;
        b = NULL;
    }

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
Vector3<float> *Triangle::getNormal() { return normal; }



/* ### Rectangle ### */

/// Rectangle class constructor
/// \param p1 First point
/// \param p2 Second point
/// \param p3 Third point
/// \param p4 Fourth point
/// \param ambCol Ambient colour of the shape
/// \param diffCol Diffuse colour of the shape
/// \param specCol Specular colour of the shape
/// \param ambCoe Ambient coefficient of the shape
/// \param diffCoe Diffuse coefficient of the shape
/// \param specCoe Specular coefficient of the shape
/// \param phongCoe Phone coefficient of the shape
Rectangle::Rectangle(Vector3<float> *p1, Vector3<float> *p2, Vector3<float> *p3, Vector3<float> *p4,
        Vector3<float> *ambCol, Vector3<float> *diffCol, Vector3<float> *specCol,
        float ambCoe, float diffCoe, float specCoe,
        float phongCoe) :
    Shape("Rectangle", ambCol, diffCol, specCol, ambCoe, diffCoe, specCoe, phongCoe),
    t1(new Triangle(p1, p2, p3, ambCol, diffCol, specCol, ambCoe, diffCoe, specCoe, phongCoe, false)) {

    // Checking to see where the hypotenuse is
    float p1p2 = (*p1 - *p2).norm();
    float p2p3 = (*p2 - *p3).norm();
    float p3p1 = (*p3 - *p1).norm();

    // Creating the second triangle along the hypotenuse
    if (p1p2 >= p2p3 && p1p2 >= p3p1) {
        t2 = new Triangle(p1, p2, p4, ambCol, diffCol, specCol, ambCoe, diffCoe, specCoe, phongCoe, true);
    }
    else if (p2p3 >= p1p2 && p2p3 >= p3p1) {
        t2 = new Triangle(p2, p3, p4, ambCol, diffCol, specCol, ambCoe, diffCoe, specCoe, phongCoe, true);
    }
    else if (p3p1 >= p1p2 && p3p1 >= p2p3) {
        t2 = new Triangle(p3, p1, p4, ambCol, diffCol, specCol, ambCoe, diffCoe, specCoe, phongCoe, true);
    }
}

/// Rectangle destructor
Rectangle::~Rectangle() {
    delete t1;
    t1 = NULL;

    delete t2;
    t2 = NULL;

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
Vector3<float> *Rectangle::A() { return t1->A(); }
/// Second point getter
/// \return Second point
Vector3<float> *Rectangle::B() { return t1->B(); }
/// Third point getter
/// \return Third point
Vector3<float> *Rectangle::C() { return t1->C(); }
/// Fourth point getter
/// \return Fourth point
Vector3<float> *Rectangle::D() { return t2->C(); }
/// Normal getter
/// \return Normal to the plane
Vector3<float> *Rectangle::getNormal() { return t1->getNormal(); }



/* ### Shape3D ### */

/// Shape3D abstract class constructor
/// \param t The type of the shape
/// \param o The origin of the shape
/// \param ambCol Ambient colour of the shape
/// \param diffCol Diffuse colour of the shape
/// \param specCol Specular colour of the shape
/// \param ambCoe Ambient coefficient of the shape
/// \param diffCoe Diffuse coefficient of the shape
/// \param specCoe Specular coefficient of the shape
/// \param phongCoe Phone coefficient of the shape
Shape3D::Shape3D(string t, Vector3<float> *o,
        Vector3<float> *ambCol, Vector3<float> *diffCol, Vector3<float> *specCol,
        float ambCoe, float diffCoe, float specCoe,
        float phongCoe) :
    Shape(t, ambCol, diffCol, specCol, ambCoe, diffCoe, specCoe, phongCoe), origin(o) { }

/// Shape3D destructor
Shape3D::~Shape3D() {
    delete origin;
    origin = NULL;
}

/// Origin getter
/// \return The origin of the shape
Vector3<float> *Shape3D::getOrigin() { return origin; }



/* ### Sphere $$$ */

/// Sphere class constructor
/// \param o The origin of the shape
/// \param r Radius of the sphere
/// \param ambCol Ambient colour of the shape
/// \param diffCol Diffuse colour of the shape
/// \param specCol Specular colour of the shape
/// \param ambCoe Ambient coefficient of the shape
/// \param diffCoe Diffuse coefficient of the shape
/// \param specCoe Specular coefficient of the shape
/// \param phongCoe Phone coefficient of the shape
Sphere::Sphere(Vector3<float> *o, float r,
        Vector3<float> *ambCol, Vector3<float> *diffCol, Vector3<float> *specCol,
        float ambCoe, float diffCoe, float specCoe,
        float phongCoe) :
    Shape3D("Sphere", o, ambCol, diffCol, specCol, ambCoe, diffCoe, specCoe, phongCoe), radius(r) { }

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
float Sphere::getRadius() { return radius; }