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
        if (l->get_type() == "Point") {
            Point *temp = dynamic_cast<Point*>(l);

            delete temp;
            temp = NULL;
        }
        else if (l->get_type() == "Area") {
            Area *temp = dynamic_cast<Area*>(l);

            delete temp;
            temp = NULL;
        }
    }

    delete output;
    output = NULL;

    cout << "delete RAYTRACER" << endl;
}

/// Main method to actually run the RayTracer
void RayTracer::run() {
    cout << endl << "Scene contains:" << endl;

    shapes = parse_geometry();
    lights = parse_lights();
    output = parse_output();

    for (Shape *s : shapes)
        output->get_image()->raycast(output->get_camera(), s, true, false, true);

    cout << endl << "Number of hits: " << output->get_image()->get_number_of_hits() << endl << endl;
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
            new Vector3<float>((*itr)["up"][0], (*itr)["up"][1], (*itr)["up"][2]));
    }

    return new Output(img, cam);
}



/* ### Camera ### */

/// Camera class constructor
/// \param o Origin of the camera
/// \param l LookAt vector of the camera
/// \param u Up vector of the camera
Camera::Camera(Vector3<float> *o, Vector3<float> *l, Vector3<float> *u) :
    origin(o),
    look_at(l),
    up(u),
    side(new Vector3<float>(l->cross(*u).normalized())) { }

/// Camera destructor
Camera::~Camera() {
    delete origin;
    origin = NULL;

    delete look_at;
    look_at = NULL;

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
                << "\tLook-At: " << c.look_at->x() << " " << c.look_at->y() << " " << c.look_at->z() << " " << endl
                << "\tUp: " << c.up->x() << " " << c.up->y() << " " << c.up->z() << " " << endl
                << "\tSide: " << c.side->x() << " " << c.side->y() << " " << c.side->z();
}

/// Origin getter
/// \return Origin of the camera
Vector3<float> *Camera::get_origin() { return origin; }
/// LookAt vector getter
/// \return LookAt vector of the camera
Vector3<float> *Camera::get_look_at() { return look_at; }
/// Up vector getter
/// \return Up vector of the camera
Vector3<float> *Camera::get_up() { return up; }
/// Side vector getter
/// \return Side vector of the camera
Vector3<float> *Camera::get_side() { return side; }



/* ### Image ### */

/// Image class constructor
/// \param n Name of the image
/// \param w Width of the image
/// \param h Height of the image
/// \param f FOV of the image
/// \param amb Ambient colour of the image
/// \param back Background colour of the image
Image::Image(string n, int w, int h, float f, Vector3<float> *amb, Vector3<float> *back) :
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
string Image::get_name() { return name; }
/// Width getter
/// \return Width of the Image
int Image::get_width() const { return width; }
/// Height getter
/// \return Height of the Image
int Image::get_height() const { return height; }
/// FOV getter
/// \return FOV of the Image
float Image::get_FOV() const { return FOV; }
/// Alpha getter
/// \return Alpha of the Image
float Image::get_alpha() const { return tan((FOV / 2) * (M_PI / 180)); }
/// Pixel size getter
/// \return Pixel size of the Image
float Image::get_pixel_size() const { return (2 * get_alpha()) / height; }
/// Number of hits getter
/// \return Number of total hits in the image
int Image::get_number_of_hits() { return number_of_hits; }

/// Ambient colour getter
/// \return Ambient colour of the Image
Vector3<float> *Image::get_ambient() { return ambient; }
/// Background colour getter
/// \return Background colour of the Image
Vector3<float> *Image::get_background() { return background; }

/// Method to raycast into the scene in the hopes of hitting a particular shape
/// \param cam The camera from which to cast the rays
/// \param sha The shape that we're trying to hit
/// \param save Whether to save the resulting hits to the PPM
/// \param verbose Whether to print raycasts to the console as they happen
/// \param onlyDisplayHits Whether to display only the raycasts that hit in the console
void Image::raycast(Camera *cam, Shape *sha, bool save, bool verbose, bool onlyDisplayHits) {
    int count = 0;

    float aspect = width / height;

    Vector3<float> base =
        *cam->get_origin() + *cam->get_look_at() +
        (*cam->get_up() * get_alpha()) -
        (*cam->get_side() * get_alpha() * aspect);

    // Iterating vertically first, then horizontally
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            count++;

            Ray *ray = new Ray(this, cam, base, sha, i, j); // Creating and shooting the raycast

            // Printing the raycast information to the console
            if (verbose && ((onlyDisplayHits && ray->get_does_hit()) || !onlyDisplayHits)) {
                cout << "[" << ray->get_does_hit() << "] Raycast " << count << ": " <<
                    ray->get_raycast()->x() << " " <<
                    ray->get_raycast()->y() << " " <<
                    ray->get_raycast()->z() << " " << endl;
            }

            // Saving the pixel information to the PPM buffer if it does hit
            if (ray->get_does_hit()) {
                number_of_hits++;

                // TODO: this should be projective (not orthographic) in the future
                if (save) {
                    int index = (3 * j * width) + (3 * i);

                    Vector3<float> intensity = ray->get_intensity(); // TODO: implement this below

                    buffer->at(index + 0) = sha->get_ambient_colour()->x();
                    buffer->at(index + 1) = sha->get_ambient_colour()->y();
                    buffer->at(index + 2) = sha->get_ambient_colour()->z();
                }
            }

            delete ray;
            ray = NULL;
        }
    }

    if (save) save_ppm(name, *buffer, width, height);
}



/* ### Ray ### */

Vector3<float> getTriangleRaycast(Vector3<float> *o, Vector3<float> *n, Vector3<float> out) {
    float t = -(o->dot(*n) - 4) / (out.dot(*n));
    return *o + (out * t);
}

/// Ray class constructor
/// \param img Image to raycast into
/// \param cam Camera to raycast from
/// \param base Base vector to offset from
/// \param sha Shape to raycast at
/// \param x_index Horizontal index of the raycast
/// \param y_index Vertical index of the raycast
Ray::Ray(Image *img, Camera *cam, Vector3<float> base, Shape *sha, int x_index, int y_index) {
    Vector3<float> out =
        base +
        (*cam->get_up() * (-y_index * img->get_pixel_size() + (img->get_pixel_size() / 2))) +
        (*cam->get_side() * (x_index * img->get_pixel_size() + (img->get_pixel_size() / 2)));

    // Checking to see which shape we are raycasting at, so we can hit it properly
    if (sha->getType() == "Triangle") {
        Triangle *tri = dynamic_cast<Triangle*>(sha);

        raycast = new Vector3<float>(getTriangleRaycast(cam->get_origin(), tri->get_normal(), out));

        does_hit = hit_triangle(tri);
    }
    if (sha->getType() == "Rectangle") {
        Rectangle *pla = dynamic_cast<Rectangle*>(sha);

        raycast = new Vector3<float>(getTriangleRaycast(cam->get_origin(), pla->getT1()->get_normal(), out));
        does_hit = hit_triangle(pla->getT1());

        if (!does_hit) {
            delete raycast;
            raycast = NULL;

            raycast = new Vector3<float>(getTriangleRaycast(cam->get_origin(), pla->getT2()->get_normal(), out));
            does_hit = hit_triangle(pla->getT2());
        }
    }
    else if (sha->getType() == "Sphere") {
        raycast = new Vector3<float>(out);

        Sphere *sph = dynamic_cast<Sphere*>(sha);

        does_hit = hit_sphere(cam->get_origin(), sph);
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

/// Free method to determine whether a ray is on the right of a line
/// \param ray The ray to check
/// \param p1 The first point of the line
/// \param p2 The second point of the line
/// \param n The normal of the line
/// \return Whether the ray is on the right of the line
bool isOnRight(Vector3<float> *ray, Vector3<float> *p1, Vector3<float> *p2, Vector3<float> *n) {
    return ((*p1 - *p2).cross(*ray - *p2)).dot(*n) > 0;
}

/// Method to see if the raycast hits a triangle
/// \param tri Triangle to hit
/// \return Whether the ray does hit
bool Ray::hit_triangle(Triangle *tri) {
    bool ba = isOnRight(raycast, tri->B(), tri->A(), tri->get_normal());
    bool cb = isOnRight(raycast, tri->C(), tri->B(), tri->get_normal());
    bool ac = isOnRight(raycast, tri->A(), tri->C(), tri->get_normal());

    // Only hits if it's on the same side as all of the triangle's lines
    bool temp = (ba == cb) && (cb == ac);

    if (temp) hit = raycast; // TODO: this may not always be correct

    return temp;
}

/// Method to see if the raycast hits a sphere
/// \param o Origin of the raycast
/// \param sph Sphere to hit
/// \return Whether the ray does hit
bool Ray::hit_sphere(Vector3<float> *o, Sphere *sph) {
    Vector3<float> dir = raycast->normalized();
    Vector3<float> toSphere = *o - *sph->get_origin();

    // Getting the components of the quadratic formula
    float a = dir.dot(dir);
    float b = 2 * toSphere.dot(dir);
    float c = toSphere.dot(toSphere) - (sph->get_radius() * sph->get_radius());

    // Getting the critical determinant
    float determinant = sqrt((b * b) - 4 * a * c);

    // Finding the two possible roots
    float root1 = (-b + sqrt(determinant)) / (2 * a);
    float root2 = (-b - sqrt(determinant)) / (2 * a);

    // TODO: I don't think this is accurate
    // Finding which root is closer (the hit which will be displayed)
    hit = new Vector3<float>(*o + dir * (root1 < root2 ? root1 : root2));

    return determinant >= 0;
}

Vector3<float> Ray::get_intensity() {
    // TODO
    // integral of hemisphere
    // BRDF to determine how much comes out based onw what comes in (based on material)
    // incoming ray
    // attenuation factor of incoming angle (based on geometry)

    return {1, 1, 1};
}

/// Hit point getter
/// \return The hit point of the raycast
Vector3<float> *Ray::get_hit() { return hit; }
/// Raycast vector getter
/// \return The vector of the raycast
Vector3<float> *Ray::get_raycast() { return raycast; }
/// Does hit getter
/// \return Whether the raycast hits the shape
bool Ray::get_does_hit() { return does_hit; }