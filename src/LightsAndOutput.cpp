#include "Shapes.h"
#include "LightsAndOutput.h"
#include "RayTracer.h"

/* ### Light ### */

/// Light class constructor
/// \param t Type of the light
/// \param diff_int Diffuse intensity of the light
/// \param spec_int Specular intensity of the light
Light::Light(string t, Vector3f *diff_int, Vector3f *spec_int) :
    type(t), diffuse_intensity(diff_int), specular_intensity(spec_int) { }

/// Light destructor
Light::~Light() {
    delete diffuse_intensity;
    diffuse_intensity = NULL;

    delete specular_intensity;
    specular_intensity = NULL;

    //cout << "delete LIGHT" << endl;
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

    //cout << "delete POINT LIGHT" << endl;
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
/// \param stratified_n Stratification array
/// \param center Whether or not to use the centre of the light for calculations
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
/// Stratification array getter
/// \return Stratification array
int Area::get_n() { return n; }
/// Use center getter
/// \return Whether or not to use the centre of the light for calculations
bool Area::get_usecenter() { return usecenter; }



/* ### Camera ### */

/// Camera class constructor
/// \param o Origin of the camera
/// \param l LookAt vector of the camera
/// \param u Up vector of the camera
/// \param f FOV of the camera
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

    //cout << "delete CAMERA" << endl;
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
/// FOV getter
/// \return FOV of the camera
float Camera::get_FOV() const { return FOV; }



/* ### Image ### */

/// Image class constructor
/// \param n Name of the image
/// \param w Width of the image
/// \param h Height of the image
/// \param amb Ambient colour of the image
/// \param back Background colour of the image
/// \param rays Number of rays to send per pixel
/// \param anti Whether or not to use antialiasing
/// \param global Whether or not to use global illumination
/// \param max Maximum number of global illumination bounces
/// \param terminate Probability for global illumination bounce termination
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
    // Setting the default colour of the background
    for (int i = 0; i < buffer->size(); i += 3) {
        buffer->at(i + 0) = globalillum ? 0 : background->x();
        buffer->at(i + 1) = globalillum ? 0 : background->y();
        buffer->at(i + 2) = globalillum ? 0 : background->z();
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

    //cout << "delete IMAGE (" << name << ")" << endl;
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

/// Ambient colour getter
/// \return Ambient colour of the Image
Vector3f *Image::get_ambient() { return ambient; }
/// Background colour getter
/// \return Background colour of the Image
Vector3f *Image::get_background() { return background; }

/// Rays per pixel getter
/// \return Number of rays to send per pixel
vector<int> Image::get_raysperpixel() { return raysperpixel; }
/// Antialiasing getter
/// \return Whether or not to use antialiasing
bool Image::get_antialiasing() { return antialiasing; }
/// Global illumination getter
/// \return Whether or not to use global illumination
bool Image::get_globalillum() { return globalillum; }

/// Quick method to calculate the starting offset for the raycasts
/// \param cam The Camera to raycast from
/// \return The initial raycast position to offset from
Vector3f Image::get_base(Camera* cam) {
    float alpha = get_alpha(cam);
    float aspect = width / height;

    Vector3f top = *cam->get_up() * alpha;
    Vector3f right = *cam->get_side() * alpha * aspect;

    return *cam->get_origin() + *cam->get_look_at() + top - right;
}

/// Quick method to check if the global illumination bounces should terminate early
/// \param probterminate Probability for global illumination bounce termination
/// \return Whether or not to terminate the bounces
bool check_probterminate(float probterminate) {
    random_device dev;
    mt19937 rng(dev());
    uniform_real_distribution<float> dist(0, 1);

    return dist(rng) <= probterminate;
}

/// Calculates a new direction for a global illumination bounce to go in
/// \param o Origin of the bounce
/// \param n Normal of the hemisphere at the bounce
/// \return The new bounce direction
Vector3f get_new_bounce_direction(Vector3f o, Vector3f n) {
    random_device dev;
    mt19937 rng(dev());
    uniform_real_distribution<float> dist(-1, 1);

    float x = dist(rng);
    float y = dist(rng);

    // Making sure the x and y are within the circle
    while (pow(x, 2) + pow(y, 2) > 1) {
        x = dist(rng);
        y = dist(rng);
    }

    float z = sqrt(1 - pow(x, 2) - pow(y, 2));;

    // Calculating the rotation to get from up to the hemisphere normal
    Matrix3f rotation = Quaternionf().setFromTwoVectors(Vector3f(0, 1, 0), n).toRotationMatrix();

    // Rotating the new direction so it aligns with the normal
    return rotation * Vector3f(x, y, z);
}

/// Determines the cosine of the angle between an incoming and outgoing vector
/// \param in Incoming vector
/// \param out Outgoing vector
/// \return The cosine of the angle between the given vectors
float get_cos_angle(Vector3f in, Vector3f out) {
    float angle = acos(in.dot(out) / (in.norm() * out.norm()));
    return cos(angle);
}

/// Method to raycast into the scene in the hopes of hitting a particular shape
/// \param cam The camera from which to cast the rays
/// \param img The image to generate to
/// \param sha The shape that we're trying to hit
/// \param all_lights List of lights to include in intensity calculations
/// \param all_shapes List of shapes to include in the calculations
void Image::raycast(
        Camera *cam,
        Image *img,
        Shape *sha,
        vector<Light*> all_lights,
        vector<Shape*> all_shapes) {
    Vector3f base = get_base(cam);
    float pixel_size = img->get_pixel_size(cam);

    Vector3f *last_hit, *last_normal, *last_dir;
    Shape *last_shape;

    int num_cells_x = 1;
    int num_cells_y = 1;
    int num_rays = 1;

    if (globalillum || (!globalillum && antialiasing)) {
        // Checking the raysperpixel to see what kind of stratification to use
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
    }

    int num_samples = num_cells_x * num_cells_y * num_rays; // Total number of samples that will be calculated

    random_device dev;
    mt19937 rng(dev());

    // Iterating vertically first, then horizontally
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            vector<Vector3f> all_sample_colours; // List of all colours generated from samples for this pixel
            bool did_all_firsts_miss = true; // Whether all samples in this pixel first missed

            // Looping through all the cells for this pixel
            for (int current_cell_x = 0; current_cell_x < num_cells_x; current_cell_x++) {
                for (int current_cell_y = 0; current_cell_y < num_cells_y; current_cell_y++) {
                    // Shooting a number of rays per cell
                    for (int current_ray = 0; current_ray < num_rays; current_ray++) {
                        Vector3f average_cell_colour(0, 0, 0); // Average colour of this cell
                        Vector2f offset(0, 0);

                        if (globalillum || (!globalillum && antialiasing)) {
                            // Generating a new offset for the sample within the current cell
                            uniform_real_distribution<float> dist_x(0, 1.0 / (1.0 / num_cells_x));
                            uniform_real_distribution<float> dist_y(0, 1.0 / (1.0 / num_cells_y));
                            offset = Vector2f(dist_x(rng), dist_y(rng)); // Offset percentages (of one pixel)
                        }

                        // Calculating each new outgoing ray + the offset
                        Vector3f initial_outgoing_position = base
                            - (*cam->get_up() * ((j + offset.y()) * pixel_size + (pixel_size / 2)))
                            + (*cam->get_side() * ((i + offset.x()) * pixel_size + (pixel_size / 2)));

                        Vector3f initial_outgoing_direction = (initial_outgoing_position - *cam->get_origin()).normalized();

                        // Creating and shooting the initial raycast
                        Ray *ray = new Ray(
                            *cam->get_origin(),
                            initial_outgoing_direction,
                            sha
                        );

                        // If the first ray hits, start bouncing
                        if (ray->get_does_hit()) {
                            did_all_firsts_miss = false;

                            // Getting the hit point, normal of the hit, and incoming direction of the first ray
                            last_hit = new Vector3f(*ray->get_hit());
                            last_normal = new Vector3f(*ray->get_hit_normal());
                            last_dir = new Vector3f((*ray->get_hit() - *cam->get_origin()).normalized());
                            last_shape = ray->get_hit_shape();

                            // Adding the intensity of the first hit
                            average_cell_colour += ray->get_average_intensity(
                                last_hit,
                                sha,
                                all_shapes,
                                all_lights,
                                globalillum
                            );

                            if (globalillum) {
                                int bounces = 0; // The current number of bounces
                                // Whether to keep bouncing
                                bool keep_bouncing = bounces < maxbounces && check_probterminate(probterminate);
                                bool last_bounce = false; // Whether this bounce is the last one that goes to the light

                                // Whether the bounces haven't ended yet or if this is the last bounce
                                while (keep_bouncing || (!keep_bouncing && !last_bounce)) {
                                    bounces++;

                                    // Updating the bouncing booleans
                                    if (keep_bouncing)
                                        keep_bouncing = bounces < maxbounces && check_probterminate(probterminate);
                                    else last_bounce = true;

                                    Vector3f new_dir;

                                    // If there are still more bounces to go, the new bounce direction is calculated
                                    if (!last_bounce) new_dir = get_new_bounce_direction(*last_hit, *last_normal);
                                        // If this is the last bounce, the direction goes to a random light
                                    else {
                                        uniform_int_distribution<int> bounce_dist(0, all_lights.size() - 1);

                                        Light *l = all_lights[bounce_dist(rng)];
                                        Vector3f light_pos;

                                        // Checking whether the randomly-selected light is a Point or Area light
                                        if (l->get_type() == "Point") {
                                            light_pos = *dynamic_cast<Point*>(l)->get_origin();
                                        }
                                        else if (l->get_type() == "Area") {
                                            Area *area = dynamic_cast<Area*>(l);
                                            light_pos = (*area->P3() - *area->P1()) / 2; // Using the middle point of the area light
                                        }

                                        new_dir = (light_pos - *last_hit).normalized();
                                    }

                                    Ray *bounce_ray;
                                    float min = INT_MAX;

                                    // Once the direction has been found, it must be checked against all the geometry
                                    for (int k = 0; k < all_shapes.size(); k++) {
                                        Ray *temp_ray;

                                        // Creating a new temporary ray for each geometry
                                        temp_ray = new Ray(
                                            *last_hit,
                                            new_dir,
                                            all_shapes[k]
                                        );

                                        // Setting the bounce ray initially
                                        // (in case nothing else hits)
                                        if (k == 0) bounce_ray = temp_ray;

                                        // If the temporary ray does it, it's checked to see if it's the closest hit
                                        // (also making sure it isn't hitting itself again)
                                        if (temp_ray->get_does_hit() && temp_ray->get_hit_shape() != last_shape) {
                                            float distance = (*temp_ray->get_hit() - *last_hit).norm();

                                            if (distance < min) {
                                                // Deleting the previously set bounce ray
                                                // (because we're going to replace it)
                                                if (k > 0) {
                                                    delete bounce_ray;
                                                    bounce_ray = NULL;
                                                }

                                                min = distance;
                                                bounce_ray = temp_ray; // Setting the new bounce ray
                                            }
                                                // Otherwise, deleting the temporary ray
                                            else {
                                                if (k > 0) {
                                                    delete temp_ray;
                                                    temp_ray = NULL;
                                                }
                                            }
                                        }
                                            // Otherwise, deleting the temporary ray
                                        else {
                                            if (k > 0) {
                                                delete temp_ray;
                                                temp_ray = NULL;
                                            }
                                        }
                                    }

                                    // Temporarily saving the incoming direction of the last bounce ray
                                    // (so it doesn't get removed by the deletes below)
                                    Vector3f temp = *last_dir;

                                    // Saving the last bounce's direction
                                    // (it'll be used below for the attenuation calculation)
                                    if (bounce_ray->get_does_hit()) {
                                        delete last_dir;
                                        last_dir = NULL;

                                        last_dir = new Vector3f((*bounce_ray->get_hit() - temp).normalized());
                                    }

                                    delete last_hit;
                                    last_hit = NULL;

                                    delete last_normal;
                                    last_normal = NULL;

                                    // If the new bounce does in fact hit something
                                    if (bounce_ray->get_does_hit()) {
                                        // If this is the last bounce, that means that something's in the way
                                        // (this sample will be in shadow)
                                        if (last_bounce) {
                                            average_cell_colour = Vector3f(0, 0, 0);
                                            break;
                                        }

                                        // Setting the new last hit and normal variables
                                        last_hit = new Vector3f(*bounce_ray->get_hit());
                                        last_normal = new Vector3f(*bounce_ray->get_hit_normal());
                                        last_shape = bounce_ray->get_hit_shape();

                                        // Getting the diffuse colour of this bounce hit
                                        Vector3f diffuse_colour = *bounce_ray->get_hit_shape()->get_diffuse_colour();

                                        // Multiplying the average cell colour by:
                                        // the diffuse colour (dc), the attenuation factor, and the diffuse coefficient (kd)
                                        average_cell_colour =
                                            Vector3f(
                                                average_cell_colour.x() * diffuse_colour.x(),
                                                average_cell_colour.y() * diffuse_colour.y(),
                                                average_cell_colour.z() * diffuse_colour.z())
                                            * get_cos_angle(-*last_dir, new_dir)
                                            * bounce_ray->get_hit_shape()->get_diffuse_coefficient();
                                    }
                                    else {
                                        // If this is isn't the last bounce, this means it didn't hit anything
                                        // (this sample shot off into space)
                                        if (!last_bounce) {
                                            average_cell_colour = Vector3f(0, 0, 0);
                                            break;
                                        }
                                    }

                                    delete bounce_ray;
                                    bounce_ray = NULL;
                                }
                            }

                            // Deleting all of the bouncing variables after the bouncing has finished

                            delete last_hit;
                            last_hit = NULL;

                            delete last_normal;
                            last_normal = NULL;

                            delete last_dir;
                            last_dir = NULL;
                        }
                        // if the first rays doesn't hit, add { 0, 0, 0 } and move along
                        else {
                            average_cell_colour = Vector3f(0, 0, 0);
                        }

                        // Deleting the first ray
                        delete ray;
                        ray = NULL;

                        all_sample_colours.push_back(average_cell_colour); // Adding the new average sample colour
                    }
                }
            }

            // If at least one of the samples did initially hit, we can calculate the colour
            if (!did_all_firsts_miss) {
                Vector3f pixel_average(0, 0, 0);

                for (Vector3f colour : all_sample_colours) {
                    pixel_average += colour;
                }

                pixel_average /= all_sample_colours.size(); // Getting an average of all the samples from this pixel

                int index = (3 * j * width) + (3 * i);
                buffer->at(index + 0) = pixel_average.x();
                buffer->at(index + 1) = pixel_average.y();
                buffer->at(index + 2) = pixel_average.z();
            }
        }
    }

    save_ppm(name, *buffer, width, height); // Saving to the PPM
}



/* ### Output ### */

/// Output container class constructor
/// \param img Scene image
/// \param cam Scene camera
Output::Output(Camera *cam, Image *img) : camera(cam), image(img) { }

/// Output destructor
Output::~Output() {
    delete camera;
    camera = NULL;

    delete image;
    image = NULL;

    //cout << "delete OUTPUT" << endl;
}

/// Scene camera getter
/// \return Scene camera
Camera *Output::get_camera() { return camera; }
/// Scene image getter
/// \return Scene image
Image *Output::get_image() { return image; }