#include "Shapes.h"
#include "LightsAndOutput.h"

/* ### Camera ### */

/// Camera class constructor
/// \param o Origin of the camera
/// \param l LookAt vector of the camera
/// \param u Up vector of the camera
Camera::Camera(Vector3f *o, Vector3f *l, Vector3f *u, float f) :
    origin(o),
    look_at(l),
    up(u),
    side(new Vector3f(l->cross(*u).normalized())),
    FOV(f) { }

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
Vector3f *Camera::get_origin() { return origin; }
/// LookAt vector getter
/// \return LookAt vector of the camera
Vector3f *Camera::get_look_at() { return look_at; }
/// Up vector getter
/// \return Up vector of the camera
Vector3f *Camera::get_up() { return up; }
/// Side vector getter
/// \return Side vector of the camera
Vector3f *Camera::get_side() { return side; }
float Camera::get_FOV() const { return FOV; }



/* ### Image ### */

/// Image class constructor
/// \param n Name of the image
/// \param w Width of the image
/// \param h Height of the image
/// \param f FOV of the image
/// \param amb Ambient colour of the image
/// \param back Background colour of the image
Image::Image(
    string n,
    int w, int h,
    Vector3f *amb, Vector3f *back,
    vector<int> rays,
    bool anti, bool global,
    int max, float terminate) :
        name(n),
        width(w), height(h),
        buffer(new vector<double>(3 * w * h)),
        ambient(amb), background(back),
        raysperpixel(rays),
        antialiasing(anti), globalillum(global),
        maxbounces(max), probterminate(terminate) {
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
                << "\tWidth: " << i.width << endl;
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
/// Alpha getter
/// \return Alpha of the Image
float Image::get_alpha(Camera* cam) const { return tan((cam->get_FOV() / 2) * (M_PI / 180)); }
/// Pixel size getter
/// \return Pixel size of the Image
float Image::get_pixel_size(Camera* cam) const { return (2 * get_alpha(cam)) / height; }
/// Number of hits getter
/// \return Number of total hits in the image
int Image::get_number_of_hits() { return number_of_hits; }

/// Ambient colour getter
/// \return Ambient colour of the Image
Vector3f *Image::get_ambient() { return ambient; }
/// Background colour getter
/// \return Background colour of the Image
Vector3f *Image::get_background() { return background; }

vector<int> Image::get_raysperpixel() { return raysperpixel; }
bool Image::get_antialiasing() { return antialiasing; }
bool Image::get_globalillum() { return globalillum; }

void Image::local_raycast(
        Camera *cam,
        Image *img,
        Shape *sha,
        vector<Light*> li,
        bool verbose,
        bool only_display_hits,
        vector<Shape*> all_shapes) {
    float aspect = width / height;
    float alpha = get_alpha(cam);
    float pixel_size = img->get_pixel_size(cam);

    Vector3f base =
        *cam->get_origin() + *cam->get_look_at() +
        (*cam->get_up() * alpha) -
        (*cam->get_side() * alpha * aspect);

    int count = 0;

    // Iterating vertically first, then horizontally
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int index = (3 * j * width) + (3 * i);

            Vector3f outgoing_ray =
                base +
                (*cam->get_up() * (-j * pixel_size + (pixel_size / 2))) +
                (*cam->get_side() * (i * pixel_size + (pixel_size / 2)));

            // Creating and shooting the raycast
            Ray *ray = new Ray(*cam->get_origin(), outgoing_ray, sha, false);

            // Printing the raycast information to the console
            if (verbose && ((only_display_hits && ray->get_does_hit()) || !only_display_hits)) {
                cout << "[" << ray->get_does_hit() << "] Raycast " << count << ": " <<
                     ray->get_raycast()->x() << " " <<
                     ray->get_raycast()->y() << " " <<
                     ray->get_raycast()->z() << " " << endl;
            }

            // Saving the pixel information to the PPM buffer if it does hit
            if (ray->get_does_hit()) {
                number_of_hits++;

                Vector3f intensity(0, 0, 0);

                for (Light* l : li) {
                    if (l->get_type() == "Point") {
                        intensity += ray->get_intensity(
                            ray->get_hit(),
                            sha,
                            *dynamic_cast<Point *>(l)->get_origin(),
                            sha->get_phong_coefficient(),
                            all_shapes);
                    } else if (l->get_type() == "Area") {
                        Area *area = dynamic_cast<Area *>(l);

                        intensity += ray->get_intensity(
                            ray->get_hit(),
                            sha,
                            (*area->P3() - *area->P1()) / 2,
                            sha->get_phong_coefficient(),
                            all_shapes);
                    }
                }

                intensity /= li.size();

                buffer->at(index + 0) = intensity.x();
                buffer->at(index + 1) = intensity.y();
                buffer->at(index + 2) = intensity.z();
            }

            delete ray;
            ray = NULL;

            count++;
        }
    }

    save_ppm(name, *buffer, width, height);
}

Vector2f Image::get_stratified_offset(float max_x, float max_y) {
    random_device dev;
    mt19937 rng(dev());
    uniform_real_distribution<float> dist_x(0, 1 / max_x);
    uniform_real_distribution<float> dist_y(0, 1 / max_y);

    return Vector2f(dist_x(rng), dist_y(rng));
}

Vector3f Image::get_initial_outgoing_global_ray(Camera* cam, Vector2f offset_max, Vector3f origin, Vector2f index, float pixel_size) {
    Vector2f offset = get_stratified_offset(offset_max.x(), offset_max.y());

    return origin +
        (*cam->get_up() * ((-index.y() + offset.y()) * pixel_size + (pixel_size / 2))) +
        (*cam->get_side() * ((index.x() + offset.x()) * pixel_size + (pixel_size / 2)));
}

bool Image::check_probterminate() {
    random_device dev;
    mt19937 rng(dev());
    uniform_real_distribution<float> dist(0, 1);

    return dist(rng) <= probterminate;
}

void Image::global_raycast(
        Camera *cam,
        Image *img,
        Shape *sha,
        vector<Light*> li,
        bool verbose,
        bool only_display_hits,
        vector<Shape*> all_shapes) {
    float aspect = width / height;
    float alpha = get_alpha(cam);
    float pixel_size = img->get_pixel_size(cam);

    Vector3f base =
        *cam->get_origin() + *cam->get_look_at() +
        (*cam->get_up() * alpha) -
        (*cam->get_side() * alpha * aspect);

    Vector3f* last_hit;

    int num_cells_x = 1;
    int num_cells_y = 1;
    int num_rays = 1;

    switch (raysperpixel.size()) {
        case 0:
            return;
        case 1:
            num_rays = raysperpixel[0];
            break;
        case 2:
            num_cells_x = raysperpixel[0];
            num_cells_y = raysperpixel[0];
            num_rays = raysperpixel[1];
            break;
        case 3:
            num_cells_x = raysperpixel[0];
            num_cells_y = raysperpixel[1];
            num_rays = raysperpixel[2];
            break;
    }

    int num_samples = num_cells_x * num_cells_y * num_rays;

    // Iterating vertically first, then horizontally
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int index = (3 * j * width) + (3 * i);

            Vector3f average_colour(0, 0, 0);

            for (int current_cell_x = 0; current_cell_x < num_cells_x; current_cell_x++) {
                for (int current_cell_y = 0; current_cell_y < num_cells_y; current_cell_y++) {
                    for (int current_ray = 0; current_ray < num_rays; current_ray++) {
                        // Creating and shooting the raycast
                        Ray *ray = new Ray(
                            *cam->get_origin(),
                            get_initial_outgoing_global_ray(
                                cam,
                                Vector2f(1 / num_cells_x, 1 / num_cells_y),
                                base,
                                Vector2f(i, j),
                                pixel_size),
                            sha,
                            true
                        );

                        last_hit = new Vector3f(*ray->get_hit());

                        int bounces = 0;

                        // Saving the pixel information to the PPM buffer if it does hit
                        if (ray->get_does_hit()) {
                            number_of_hits++;

                            while (bounces < maxbounces && check_probterminate()) {
                                bounces++;

                                Vector3f new_dir; // TODO: calculate bounces

                                Ray *bounce_ray = new Ray(
                                    *last_hit,
                                    new_dir,
                                    sha,
                                    true
                                );

                                delete last_hit;
                                last_hit = NULL;

                                if (bounce_ray->get_does_hit()) {
                                    last_hit = new Vector3f(*bounce_ray->get_hit());

                                    // TODO: calculate light at each bounce (make sure to check for usecenter)
                                    // TODO: multiply light through
                                }
                                else {
                                    // TODO: return 0, 0, 0
                                    // TODO: include in calculation later
                                    break;
                                }

                                delete bounce_ray;
                                bounce_ray = NULL;
                            }
                        }
                        else {
                            // TODO: return 0, 0, 0
                            // TODO: include in calculation later
                        }

                        if (true) { // TODO: if the last didn't miss
                            // TODO: last ray goes to the light
                        }
                        else {
                            delete last_hit;
                            last_hit = NULL;
                        }

                        delete ray;
                        ray = NULL;
                    }
                }
            }

            //average_colour /= num_samples; // TODO: better average

            buffer->at(index + 0) = average_colour.x();
            buffer->at(index + 1) = average_colour.y();
            buffer->at(index + 2) = average_colour.z();
        }
    }

    save_ppm(name, *buffer, width, height);
}

/// Method to raycast into the scene in the hopes of hitting a particular shape
/// \param cam The camera from which to cast the rays
/// \param sha The shape that we're trying to hit
/// \param li List of lights to include in intensity calculations
/// \param verbose Whether to print raycasts to the console as they happen
/// \param only_display_hits Whether to display only the raycasts that hit in the console
void Image::raycast(
        Camera *cam,
        Image *img,
        Shape *sha,
        vector<Light*> li,
        bool verbose,
        bool only_display_hits,
        vector<Shape*> all_shapes) {
    if (globalillum) global_raycast(cam, img, sha, li, verbose, only_display_hits, all_shapes);
    else local_raycast(cam, img, sha, li, verbose, only_display_hits, all_shapes);
}



/* ### Light ### */

/// Light class constructor
/// \param t Type of the light
/// \param diff_int Diffuse intensity of the light
/// \param spec_int Specular intensity of the light
Light::Light(string t, Vector3f *diff_int, Vector3f *spec_int) :
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
Vector3f *Light::get_diffuse_intensity() { return diffuse_intensity; }
/// Specular intensity getter
/// \return Specular intensity of the light
Vector3f *Light::get_specular_intensity() { return specular_intensity; }



/* ### Point ### */

/// Point light class constructor
/// \param o Origin of the light
/// \param diff_int Diffuse intensity of the light
/// \param spec_int Specular intensity of the light
Point::Point(Vector3f *o, Vector3f *diff_int, Vector3f *spec_int) :
    Light("Point", diff_int, spec_int), origin(o) { }

/// Point light destructor
Point::~Point() {
    delete origin;
    origin = NULL;

    cout << "delete POINT LIGHT" << endl;
}

/// Origin getter
/// \return Origin of the light
Vector3f *Point::get_origin() { return origin; }



/* ### Area ### */

/// Area light class constructor
/// \param a First point
/// \param b Second point
/// \param c Third point
/// \param d Fourth point
/// \param diff_int Diffuse intensity of the light
/// \param spec_int Specular intensity of the light
Area::Area(
        Vector3f* a, Vector3f* b, Vector3f* c, Vector3f* d,
        Vector3f *diff_int, Vector3f *spec_int, int stratified_n, bool center) :
    Light("Area", diff_int, spec_int),
    p1(a), p2(b), p3(c), p4(d), n(stratified_n), usecenter(center) { }

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
Vector3f *Area::P1() { return p1; }
/// Second point getter
/// \return Second point
Vector3f *Area::P2() { return p2; }
/// Third point getter
/// \return Third point
Vector3f *Area::P3() { return p3; }
/// Fourth point getter
/// \return Fourth point
Vector3f *Area::P4() { return p4; }



/* ### Output ### */

/// Output container class constructor
/// \param img Scene image
/// \param cam Scene camera
Output::Output(Camera *cam, Image *img) : camera(cam), image(img) { }

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