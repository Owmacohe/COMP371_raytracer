#include "RayTracer.h"
#include "Shapes.h"
#include "LightsAndOutput.h"

/* ### RayTracer ### */

/// RayTracer class constructor
/// \param js JSON to parse from
RayTracer::RayTracer(json js) : j(new json(js)) { }

/// RayTracer destructor
RayTracer::~RayTracer() {
    cout << endl;

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

    for (Output *o : outputs) {
        delete o;
        o = NULL;
    }

    cout << "delete RAYTRACER" << endl;
}

/// Main method to actually run the RayTracer
void RayTracer::run() {
    cout << endl << "Scene contains:" << endl;

    shapes = parse_geometry();
    lights = parse_lights();
    outputs = parse_outputs();

    cout << endl;

    for (Output *o : outputs) {
        for (Shape *s : shapes) {
            o->get_image()->raycast(
                o->get_camera(),
                s,
                lights,
                true, false, true);

            cout << "Rendered: " << s->getType() << endl;
        }

        cout << endl << "[" << o->get_image()->get_name() << "] number of hits: " << o->get_image()->get_number_of_hits() << endl;
    }

    cout << endl;
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
vector<Output*> RayTracer::parse_outputs() {
    vector<Output*> temp;

    for (auto itr = (*j)["output"].begin(); itr!= (*j)["output"].end(); itr++) {
        Image *img;
        Camera *cam;

        img = new Image(
            (*itr)["filename"],
            (*itr)["size"][0], (*itr)["size"][1], (*itr)["fov"],
            new Vector3<float>((*itr)["ai"][0],(*itr)["ai"][1],(*itr)["ai"][2]),
            new Vector3<float>((*itr)["bkc"][0],(*itr)["bkc"][1],(*itr)["bkc"][2]));

        cam = new Camera(
            new Vector3<float>((*itr)["centre"][0], (*itr)["centre"][1], (*itr)["centre"][2]),
            new Vector3<float>((*itr)["lookat"][0], (*itr)["lookat"][1], (*itr)["lookat"][2]),
            new Vector3<float>((*itr)["up"][0], (*itr)["up"][1], (*itr)["up"][2]));

        temp.push_back(new Output(img, cam));
    }

    return temp;
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
/// \param li List of lights to include in intensity calculations
/// \param save Whether to save the resulting hits to the PPM
/// \param verbose Whether to print raycasts to the console as they happen
/// \param onlyDisplayHits Whether to display only the raycasts that hit in the console
void Image::raycast(Camera *cam, Shape *sha, vector<Light*> li, bool save, bool verbose, bool onlyDisplayHits) {
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

                if (save) {
                    int index = (3 * j * width) + (3 * i);

                    Vector3<float> intensity = {0, 0, 0};

                    for (Light* k : li) {
                        if (k->get_type() == "Point")
                            intensity += ray->get_intensity(
                                ray->get_hit(),
                                sha,
                                *dynamic_cast<Point*>(k)->get_origin(),
                                sha->get_phong_coefficient());
                        /*
                        else if (k->get_type() == "Area")
                            intensity += ray->get_area_intensity(
                                ray->get_hit(),
                                sha,
                                dynamic_cast<Area*>(k),
                                sha->get_phong_coefficient());
                        */
                    }

                    intensity /= li.size(); // TODO: should I average them?

                    buffer->at(index + 0) = intensity.x();
                    buffer->at(index + 1) = intensity.y();
                    buffer->at(index + 2) = intensity.z();
                }
            }

            delete ray;
            ray = NULL;
        }
    }

    if (save) save_ppm(name, *buffer, width, height);
}



/* ### Ray ### */

/// Gets the raycast hit point for a triangle
/// \param o Origin of the raycast
/// \param n Normal of the triangle
/// \param out Camera-to-image-plane outgoing vector
/// \return The 3D hit point of the triangle raycast
Vector3<float> get_triangle_raycast(Vector3<float> *o, Vector3<float> *n, Vector3<float> out) {
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

        raycast = new Vector3<float>(get_triangle_raycast(cam->get_origin(), tri->get_normal(), out));

        does_hit = hit_triangle(tri);
    }
    if (sha->getType() == "Rectangle") {
        Rectangle *pla = dynamic_cast<Rectangle*>(sha);

        raycast = new Vector3<float>(get_triangle_raycast(cam->get_origin(), pla->getT1()->get_normal(), out));
        does_hit = hit_triangle(pla->getT1());

        if (!does_hit) {
            delete raycast;
            raycast = NULL;

            raycast = new Vector3<float>(get_triangle_raycast(cam->get_origin(), pla->getT2()->get_normal(), out));
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
bool is_on_right(Vector3<float> *ray, Vector3<float> *p1, Vector3<float> *p2, Vector3<float> *n) {
    return ((*p1 - *p2).cross(*ray - *p2)).dot(*n) > 0;
}

/// Method to see if the raycast hits a triangle
/// \param tri Triangle to hit
/// \return Whether the ray does hit
bool Ray::hit_triangle(Triangle *tri) {
    bool ba = is_on_right(raycast, tri->B(), tri->A(), tri->get_normal());
    bool cb = is_on_right(raycast, tri->C(), tri->B(), tri->get_normal());
    bool ac = is_on_right(raycast, tri->A(), tri->C(), tri->get_normal());

    // Only hits if it's on the same side as all of the triangle's lines
    bool temp = (ba == cb) && (cb == ac);

    if (temp) hit = raycast;

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

    // Finding which root is closer (the hit which will be displayed)
    hit = new Vector3<float>(*o + dir * (root1 < root2 ? root1 : root2));

    return determinant >= 0;
}

/// Free method to clamp a value above a minimum
/// \param f Float to clamp
/// \param min Minimum amount for the value
/// \return The minimally-clamped value
float clamp(float f, float min) {
    if (f < min) f = min;

    return f;
}

/// Free method to clamp a value between a minimum and maximum
/// \param f Float to clamp
/// \param min Minimum amount for the value
/// \param max Maximum amount for the value
/// \return The clamped value
float clamp(float f, float min, float max) {
    if (f < min) f = min;
    if (f > max) f = max;

    return f;
}

/// Method to calculate the intensity for a particular raycast
/// \param hit The hit point for the raycast
/// \param sha The shape being hit
/// \param poi The light source point
/// \param shininess The amount of shine
/// \return An RGB intensity for the given raycast
Vector3<float> Ray::get_intensity(Vector3<float> *hit, Shape *sha, Vector3<float> poi, float shininess) {
    Vector3<float> N;

    // Get the respective normal
    if (sha->getType() == "Triangle") {
        N = *dynamic_cast<Triangle*>(sha)->get_normal();
    }
    else if (sha->getType() == "Rectangle") {
        N = -*dynamic_cast<Rectangle*>(sha)->get_normal();
    }
    else if (sha->getType() == "Sphere") {
        N = (*hit - *dynamic_cast<Sphere*>(sha)->get_origin()).normalized();
    }

    Vector3<float> L = (poi - *hit).normalized(); // Direction from the hit to the light source

    float lambertian = clamp(N.dot(L), 0); // Setting up Lambert's cosine law
    float specular;

    if (lambertian > 0) {
        Vector3<float> R = (-L) - 2 * ((-L).dot(N)) * N;
        Vector3<float> V = (-*hit).normalized();
        Vector3<float> H = (L + V) / (L + V).norm();

        // Getting the specular
        float specAngle = clamp(H.dot(N), 0);
        specular = pow(specAngle, shininess);
    }

    // Getting the three light/colour values
    Vector3<float> amb = sha->get_ambient_coefficient() * *sha->get_ambient_colour();
    Vector3<float> diff = sha->get_diffuse_coefficient() * lambertian * *sha->get_diffuse_colour();
    Vector3<float> spec = sha->get_specular_coefficient() * specular * *sha->get_specular_colour();

    Vector3<float> intensity = amb + diff + spec;

    // Clamping each intensity value between 0 and 1
    intensity.x() = clamp(intensity.x(), 0, 1);
    intensity.y() = clamp(intensity.y(), 0, 1);
    intensity.z() = clamp(intensity.z(), 0, 1);

    return intensity;
}

Vector3<float> Ray::get_area_intensity(Vector3<float> *hit, Shape *sha, Area *area, float shininess) {
    float p1p2 = (*area->P1() - *area->P2()).norm();
    float p1p3 = (*area->P1() - *area->P3()).norm();
    float p1p4 = (*area->P1() - *area->P4()).norm();

    Vector3<float> temp1, temp2;

    if (p1p2 >= p1p3 && p1p2 >= p1p4) {
        temp1 = *area->P3();
        temp2 = *area->P4();
    }
    else if (p1p3 >= p1p2 && p1p3 >= p1p4) {
        temp1 = *area->P2();
        temp2 = *area->P4();
    }
    else if (p1p4 >= p1p2 && p1p4 >= p1p3) {
        temp1 = *area->P2();
        temp2 = *area->P3();
    }

    Vector3<float> toTemp1 = temp1 - *area->P1();
    float width = toTemp1.norm();
    Vector3<float> toTemp2 = temp2 - *area->P1();
    float height = toTemp2.norm();

    Vector3<float> intensity = {0, 0, 0};

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Vector3<float> dir = (toTemp1.normalized() * i) + (toTemp2.normalized() * j);

            intensity += get_intensity(
                get_hit(),
                sha,
                dir,
                sha->get_phong_coefficient());
        }
    }

    return intensity / (width * height);
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