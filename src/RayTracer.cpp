#include "RayTracer.h"
#include "Shapes.h"
#include "LightsAndOutput.h"

/* ### RayTracer ### */

/// RayTracer class constructor
/// \param js JSON to parse from
RayTracer::RayTracer(json js) : j(new json(js)) { }

/// RayTracer destructor
RayTracer::~RayTracer() {
    // TODO: figure out why these deletes are causing errors...

    /*
    delete j;
    j = NULL;

    for (Shape *s : shapes) {
        if (s->getType() == "Triangle") {
            Triangle *temp = dynamic_cast<Triangle*>(s);

            delete temp;
            temp = NULL;
        }
        else if (s->getType() == "Rectangle") {
            Rectangle *temp = dynamic_cast<Rectangle*>(s);

            delete temp;
            temp = NULL;
        }
        else if (s->getType() == "Sphere") {
            Sphere *temp = dynamic_cast<Sphere*>(s);

            delete temp;
            temp = NULL;
        }
    }

    for (Light *l : lights) {
        if (l->getType() == "Point") {
            Point *temp = dynamic_cast<Point*>(l);

            delete temp;
            temp = NULL;
        }
        else if (l->getType() == "Area") {
            Area *temp = dynamic_cast<Area*>(l);

            delete temp;
            temp = NULL;
        }
    }

    delete output;
    output = NULL;

    cout << "delete RAYTRACER" << endl;
    */
}

/// Main method to actually run the RayCaster
void RayTracer::run() {
    cout << endl << "Scene contains:" << endl;

    shapes = parse_geometry();
    lights = parse_lights();
    output = parse_output();

    for (Shape *s : shapes)
        output->getImage()->raycast(output->getCamera(), s, true, false, true);

    cout << endl << "Number of hits: " << output->getImage()->getNumberOfHits() << endl << endl;
}

/// Parser for the geometry elements
/// \param j JSON to parse from
/// \return A list of the parsed shapes
vector<Shape*> RayTracer::parse_geometry() {
    vector<Shape*> temp;

    for (auto itr = (*j)["geometry"].begin(); itr!= (*j)["geometry"].end(); itr++) {
        string type;

        if (itr->contains("type")) {
            type = (*itr)["type"].get<std::string>();
        }
        else {
            cout << "Fatal error: geometry should always contain a type!!!" << endl;
            // TODO: error checking
        }

        if (type == "sphere") {
            cout << "SPHERE" << endl;

            temp.push_back(new Sphere(
                new Vector3<float>((*itr)["centre"][0], (*itr)["centre"][1], (*itr)["centre"][2]),
                (*itr)["radius"].get<float>(),
                new Vector3<float>((*itr)["ac"][0], (*itr)["ac"][1], (*itr)["ac"][2]),
                new Vector3<float>((*itr)["dc"][0], (*itr)["dc"][1], (*itr)["dc"][2]),
                new Vector3<float>((*itr)["sc"][0], (*itr)["sc"][1], (*itr)["sc"][2]),
                (*itr)["ka"], (*itr)["kd"], (*itr)["ks"],
                (*itr)["pc"]
            ));
        }
        else if (type == "rectangle") {
            cout << "RECTANGLE" << endl;

            temp.push_back(new Rectangle(
                new Vector3<float>((*itr)["p1"][0], (*itr)["p1"][1], (*itr)["p1"][2]),
                new Vector3<float>((*itr)["p2"][0], (*itr)["p2"][1], (*itr)["p2"][2]),
                new Vector3<float>((*itr)["p3"][0], (*itr)["p3"][1], (*itr)["p3"][2]),
                new Vector3<float>((*itr)["p4"][0], (*itr)["p4"][1], (*itr)["p4"][2]),
                new Vector3<float>((*itr)["ac"][0], (*itr)["ac"][1], (*itr)["ac"][2]),
                new Vector3<float>((*itr)["dc"][0], (*itr)["dc"][1], (*itr)["dc"][2]),
                new Vector3<float>((*itr)["sc"][0], (*itr)["sc"][1], (*itr)["sc"][2]),
                (*itr)["ka"], (*itr)["kd"], (*itr)["ks"],
                (*itr)["pc"]
            ));
        }
    }

    return temp;
}

/// Parser for the light elements
/// \param j JSON to parse from
/// \return A list of the parsed lights
vector<Light*> RayTracer::parse_lights() {
    vector<Light*> temp;

    for (auto itr = (*j)["light"].begin(); itr!= (*j)["light"].end(); itr++) {
        string type;

        if (itr->contains("type")) {
            type = (*itr)["type"].get<std::string>();
        }
        else {
            cout << "Fatal error: geometry should always contain a type!!!" << endl;
            // TODO: error checking
        }

        if (type == "point") {
            cout << "POINT LIGHT" << endl;

            temp.push_back(new Point(
                new Vector3<float>((*itr)["centre"][0], (*itr)["centre"][1], (*itr)["centre"][2]),
                new Vector3<float>((*itr)["id"][0], (*itr)["id"][1], (*itr)["id"][2]),
                new Vector3<float>((*itr)["is"][0], (*itr)["is"][1], (*itr)["is"][2])
            ));
        }
        else if (type == "area") {
            cout << "AREA LIGHT" << endl;

            temp.push_back(new Area(
                new Vector3<float>((*itr)["p1"][0], (*itr)["p1"][1], (*itr)["p1"][2]),
                new Vector3<float>((*itr)["p2"][0], (*itr)["p2"][1], (*itr)["p2"][2]),
                new Vector3<float>((*itr)["p3"][0], (*itr)["p3"][1], (*itr)["p3"][2]),
                new Vector3<float>((*itr)["p4"][0], (*itr)["p4"][1], (*itr)["p4"][2]),
                new Vector3<float>((*itr)["id"][0], (*itr)["id"][1], (*itr)["id"][2]),
                new Vector3<float>((*itr)["is"][0], (*itr)["is"][1], (*itr)["is"][2])
            ));
        }
    }

    return temp;
}

/// Parser for the output elements
/// \param j JSON to parse from
/// \return A single output object
Output *RayTracer::parse_output() {
    Image *img;
    Camera *cam;

    for (auto itr = (*j)["output"].begin(); itr!= (*j)["output"].end(); itr++) {
        img = new Image(
            (*itr)["filename"],
            (*itr)["size"][0], (*itr)["size"][1], (*itr)["fov"],
            new Vector3<float>((*itr)["ai"][0],(*itr)["ai"][1],(*itr)["ai"][2]),
            new Vector3<float>((*itr)["bkc"][0],(*itr)["bkc"][1],(*itr)["bkc"][2]));

        cam = new Camera(
            new Vector3<float>((*itr)["centre"][0], (*itr)["centre"][1], (*itr)["centre"][2]),
            new Vector3<float>((*itr)["lookat"][0], (*itr)["lookat"][1], (*itr)["lookat"][2]),
            new Vector3<float>((*itr)["up"][0], (*itr)["up"][1], (*itr)["up"][2]),
            img->getFOV()); // TODO: should I set the focal length this way???
    }

    return new Output(img, cam);
}



/* ### Camera ### */

/// Camera class constructor
/// \param o Origin of the camera
/// \param l LookAt vector of the camera
/// \param u Up vector of the camera
/// \param f Focal length of the camera
Camera::Camera(Vector3<float> *o, Vector3<float> *l, Vector3<float> *u, float f) :
    origin(o),
    lookAt(l),
    up(u),
    side(new Vector3<float>(l->cross(*u))),
    focalLength(f) { }

/// Camera destructor
Camera::~Camera() {
    delete origin;
    origin = NULL;

    delete lookAt;
    lookAt = NULL;

    delete up;
    up = NULL;

    delete side;
    side = NULL;

    cout << "delete CAMERA" << endl;
}

/// Camera stream insertion operator
/// \param strm Incoming stream
/// \param c Camera to insert
/// \return Camera information added to the stream
ostream& operator<<(ostream &strm, const Camera &c) {
    return strm << "CAMERA" << endl
                << "\tOrigin: " << c.origin->x() << " " << c.origin->y() << " " << c.origin->z() << " " << endl
                << "\tLook-At: " << c.lookAt->x() << " " << c.lookAt->y() << " " << c.lookAt->z() << " " << endl
                << "\tUp: " << c.up->x() << " " << c.up->y() << " " << c.up->z() << " " << endl
                << "\tSide: " << c.side->x() << " " << c.side->y() << " " << c.side->z();
}

/// Origin getter
/// \return Origin of the camera
Vector3<float> *Camera::getOrigin() { return origin; }
/// LookAt vector getter
/// \return LookAt vector of the camera
Vector3<float> *Camera::getLookAt() { return lookAt; }
/// Up vector getter
/// \return Up vector of the camera
Vector3<float> *Camera::getUp() { return up; }
/// Side vector getter
/// \return Side vector of the camera
Vector3<float> *Camera::getSide() { return side; }
/// Focal length getter
/// \return Focal length of the camera
float Camera::getFocalLength() { return focalLength; }



/* ### Image ### */

/// Image class constructor
/// \param n Name of the image
/// \param w Width of the image
/// \param h Height of the image
/// \param f FOV of the image
/// \param amb Ambient colour of the image
/// \param back Background colour of the image
Image::Image(string n, float w, float h, float f, Vector3<float> *amb, Vector3<float> *back) :
    name(n),
    width(w),
    height(h),
    FOV(f),
    buffer(new vector<double>(3 * w * h)),
    ambient(amb),
    background(back) {
    for (int i = 0; i < buffer->size(); i += 3) {
        buffer->at(i + 0) = background->x();
        buffer->at(i + 1) = background->y();
        buffer->at(i + 2) = background->z();
    }
}

/// Image destructor
Image::~Image() {
    delete buffer;
    buffer = NULL;

    delete ambient;
    ambient = NULL;

    delete background;
    background = NULL;

    cout << "delete IMAGE (" << name << ")" << endl;
}

/// Image stream insertion operator
/// \param strm Incoming stream
/// \param i Image to insert
/// \return Image information added to the stream
ostream& operator<<(ostream &strm, const Image &i) {
    return strm << "IMAGE (" << i.name << ")" << endl
                << "\tHeight: " << i.height << endl
                << "\tWidth: " << i.width << endl
                << "\tFOV: " << i.FOV;
}

/// Name getter
/// \return Name of the Image
string Image::getName() { return name; }
/// Width getter
/// \return Width of the Image
float Image::getWidth() const { return width; }
/// Height getter
/// \return Height of the Image
float Image::getHeight() const { return height; }
/// FOV getter
/// \return FOV of the Image
float Image::getFOV() const { return FOV; }
/// FOV delta getter
/// \return FOV delta of the Image
float Image::getDelta() const { return (2 * tan(FOV / 2)) / height; }
/// Number of hits getter
/// \return Number of total hits in the image
int Image::getNumberOfHits() { return numberOfHits; }

/// Ambient colour getter
/// \return Ambient colour of the Image
Vector3<float> *Image::getAmbient() { return ambient; }
/// Background colour getter
/// \return Background colour of the Image
Vector3<float> *Image::getBackground() { return background; }

/// Method to raycast into the scene in the hopes of hitting a particular shape
/// \param cam The camera from which to cast the rays
/// \param sha The shape that we're trying to hit
/// \param save Whether to save the resulting hits to the PPM
/// \param verbose Whether to print raycasts to the console as they happen
/// \param onlyDisplayHits Whether to display only the raycasts that hit in the console
void Image::raycast(Camera *cam, Shape *sha, bool save, bool verbose, bool onlyDisplayHits) {
    int count = 0;

    // Iterating vertically first, then horizontally
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            count++;

            Ray *ray = new Ray(this, cam, sha, i, j); // Creating and shooting the raycast

            // Printing the raycast information to the console
            if (verbose && ((onlyDisplayHits && ray->getDoesHit()) || !onlyDisplayHits)) {
                cout << "[" << ray->getDoesHit() << "] Raycast " << count << ": " <<
                    ray->getRaycast()->x() << " " <<
                    ray->getRaycast()->y() << " " <<
                    ray->getRaycast()->z() << " " << endl;
            }

            // Saving the pixel information to the PPM buffer if it does hit
            if (ray->getDoesHit()) {
                numberOfHits++;

                // TODO: this should be projective (not orthographic) in the future
                if (save) {
                    int index = (3 * j * width) + (3 * i);

                    /*
                    int y = ray->getHit()->y() + (height / 2);
                    int x = ray->getHit()->x() + (width / 2);

                    int index = (3 * y * width) + (3 * x);
                    */

                    Vector3<float> intensity = ray->getIntensity(); // TODO: implement this below

                    buffer->at(index + 0) = sha->getAmbientColour()->x();
                    buffer->at(index + 1) = sha->getAmbientColour()->y();
                    buffer->at(index + 2) = sha->getAmbientColour()->z();
                }
            }

            delete ray;
            ray = NULL;
        }
    }

    if (save) save_ppm(name, *buffer, width, height);
}



/* ### Ray ### */

/// Free method to determine whether a ray is on the right of a line
/// \param ray The ray to check
/// \param p1 The first point of the line
/// \param p2 The second point of the line
/// \param n The normal of the line
/// \return Whether the ray is on the right of the line
bool isOnRight(Vector3<float> *ray, Vector3<float> *p1, Vector3<float> *p2, Vector3<float> *n) {
    return (*p1 - *p2).cross(*ray - *p2).dot(*n) > 0;
}

/// Ray class constructor
/// \param img Image to raycast into
/// \param cam Camera to raycast from
/// \param sha Shape to raycast at
/// \param x_index Horizontal index of the raycast
/// \param y_index Vertical index of the raycast
Ray::Ray(Image *img, Camera *cam, Shape *sha, int x_index, int y_index) {
    //Vector3<float> x = *cam->getSide() * ((img->getWidth() - (2 * x_index - 1) * img->getDelta()) / 2);
    //Vector3<float> y = *cam->getUp() * ((img->getHeight() - (2 * y_index - 1) * img->getDelta()) / 2);

    // general raycast formula: O + (l * focal) + (side * index) + (up * index)
    raycast = new Vector3<float>(
        *cam->getOrigin() +
        (*cam->getLookAt() * cam->getFocalLength()) +
        (*cam->getSide() * (-img->getWidth() / 2 + x_index)) +
        (*cam->getUp() * (img->getHeight() / 2 - y_index))
    );

    // Checking to see which shape we are raycasting at, so we can hit it properly
    if (sha->getType() == "Triangle") {
        Triangle *tri = dynamic_cast<Triangle*>(sha);

        doesHit = hitTriangle(tri);

        if (doesHit) hit = raycast;
    }
    if (sha->getType() == "Rectangle") {
        Rectangle *pla = dynamic_cast<Rectangle*>(sha);

        doesHit = hitTriangle(pla->getT1()) || hitTriangle(pla->getT2());
    }
    else if (sha->getType() == "Sphere") {
        Sphere *sph = dynamic_cast<Sphere*>(sha);

        doesHit = hitSphere(*cam->getOrigin(), sph);
    }
}

/// Ray destructor
Ray::~Ray() {
    if (hit != raycast) {
        delete hit;
        hit = NULL;
    }

    delete raycast;
    raycast = NULL;
}

/// Method to see if the raycast hits a triangle
/// \param tri Triangle to hit
/// \return Whether the ray does hit
bool Ray::hitTriangle(Triangle *tri) {
    bool ba = isOnRight(raycast, tri->B(), tri->A(), tri->getNormal());
    bool cb = isOnRight(raycast, tri->C(), tri->B(), tri->getNormal());
    bool ac = isOnRight(raycast, tri->A(), tri->C(), tri->getNormal());

    // Only hits if it's on the same side as all of the triangle's lines
    bool temp = (ba == cb) && (cb == ac);
    if (temp) hit = raycast; // TODO: this may not always be correct

    return temp;
}

/// Method to see if the raycast hits a sphere
/// \param o Origin of the raycast
/// \param sph Sphere to hit
/// \return Whether the ray does hit
bool Ray::hitSphere(Vector3<float> o, Sphere *sph) {
    Vector3<float> dir = raycast->normalized();
    Vector3<float> toSphere = o - *sph->getOrigin();

    // Getting the components of the quadratic formula
    float a = dir.dot(dir);
    float b = 2 * toSphere.dot(dir);
    float c = toSphere.dot(toSphere) - (sph->getRadius() * sph->getRadius());

    // Getting the critical determinant
    float determinant = sqrt((b * b) - 4 * a * c);

    // Finding the two possible roots
    float root1 = (-b + sqrt(determinant)) / (2 * a);
    float root2 = (-b - sqrt(determinant)) / (2 * a);

    // TODO: I don't think this is accurate
    // Finding which root is closer (the hit which will be displayed)
    hit = new Vector3<float>(o + dir * (root1 < root2 ? root1 : root2));

    return determinant >= 0;
}

Vector3<float> Ray::getIntensity() {
    // TODO
    // integral of hemisphere
    // BRDF to determine how much comes out based onw what comes in (based on material)
    // incoming ray
    // attenuation factor of incoming angle (based on geometry)

    return Vector3<float>(1, 1, 1);
}

/// Hit point getter
/// \return The hit point of the raycast
Vector3<float> *Ray::getHit() { return hit; }
/// Raycast vector getter
/// \return The vector of the raycast
Vector3<float> *Ray::getRaycast() { return raycast; }
/// Does hit getter
/// \return Whether the raycast hits the shape
bool Ray::getDoesHit() { return doesHit; }