#include "Shapes.h"
#include "LightsAndOutput.h"
#include "RayTracer.h"

/* ### RayTracer ### */

/// RayTracer class constructor
/// \param js JSON to parse from
RayTracer::RayTracer(json js) : j(new json(js)) { }

/// RayTracer destructor
RayTracer::~RayTracer() {
    cout << endl;

    delete j;
    j = NULL;

    // Deleting all the shapes
    for (Shape *s : shapes) {
        if (s->get_type() == "Triangle") {
            Triangle *temp = dynamic_cast<Triangle*>(s);

            delete temp;
            temp = NULL;
        }
        else if (s->get_type() == "Rectangle") {
            Rectangle *temp = dynamic_cast<Rectangle*>(s);

            delete temp;
            temp = NULL;
        }
        else if (s->get_type() == "Sphere") {
            Sphere *temp = dynamic_cast<Sphere*>(s);

            delete temp;
            temp = NULL;
        }
    }

    // Deleting all the lights
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

    // Deleting all the outputs
    for (Output *o : outputs) {
        delete o;
        o = NULL;
    }

    //cout << "delete RAYTRACER" << endl;
}

/// Main method to actually run the RayTracer
void RayTracer::run() {
    cout << endl << "Scene contains:" << endl;

    shapes = parse_geometry();
    lights = parse_lights();
    outputs = parse_outputs();

    cout << endl;

    for (Output *o : outputs) {
        cout << "Output: " << o->get_image()->get_name() << endl;

        for (Shape *s : shapes) {
            o->get_image()->raycast(
                o->get_camera(),
                o->get_image(),
                s,
                lights,
                shapes);

            cout << "Rendered: " << s->get_type() << endl;
        }
    }
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
        }

        bool visible = true;
        if ((*itr).contains("visible")) visible = (*itr)["visible"];

        if (visible) {
            if (type == "rectangle") {
                cout << "RECTANGLE" << endl;

                temp.push_back(new Rectangle(
                    new Vector3f((*itr)["p1"][0], (*itr)["p1"][1], (*itr)["p1"][2]),
                    new Vector3f((*itr)["p2"][0], (*itr)["p2"][1], (*itr)["p2"][2]),
                    new Vector3f((*itr)["p3"][0], (*itr)["p3"][1], (*itr)["p3"][2]),
                    new Vector3f((*itr)["p4"][0], (*itr)["p4"][1], (*itr)["p4"][2]),
                    new Vector3f((*itr)["ac"][0], (*itr)["ac"][1], (*itr)["ac"][2]),
                    new Vector3f((*itr)["dc"][0], (*itr)["dc"][1], (*itr)["dc"][2]),
                    new Vector3f((*itr)["sc"][0], (*itr)["sc"][1], (*itr)["sc"][2]),
                    (*itr)["ka"], (*itr)["kd"], (*itr)["ks"],
                    (*itr)["pc"]
                ));
            }
            else if (type == "sphere") {
                cout << "SPHERE" << endl;

                temp.push_back(new Sphere(
                    new Vector3f((*itr)["centre"][0], (*itr)["centre"][1], (*itr)["centre"][2]),
                    (*itr)["radius"].get<float>(),
                    new Vector3f((*itr)["ac"][0], (*itr)["ac"][1], (*itr)["ac"][2]),
                    new Vector3f((*itr)["dc"][0], (*itr)["dc"][1], (*itr)["dc"][2]),
                    new Vector3f((*itr)["sc"][0], (*itr)["sc"][1], (*itr)["sc"][2]),
                    (*itr)["ka"], (*itr)["kd"], (*itr)["ks"],
                    (*itr)["pc"]
                ));
            }
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
        }

        bool use = true;
        if ((*itr).contains("use")) use = (*itr)["use"];

        if (use) {
            if (type == "point") {
                cout << "POINT LIGHT" << endl;

                temp.push_back(new Point(
                    new Vector3f((*itr)["centre"][0], (*itr)["centre"][1], (*itr)["centre"][2]),
                    new Vector3f((*itr)["id"][0], (*itr)["id"][1], (*itr)["id"][2]),
                    new Vector3f((*itr)["is"][0], (*itr)["is"][1], (*itr)["is"][2])
                ));
            }
            else if (type == "area") {
                cout << "AREA LIGHT" << endl;

                temp.push_back(new Area(
                    new Vector3f((*itr)["p1"][0], (*itr)["p1"][1], (*itr)["p1"][2]),
                    new Vector3f((*itr)["p2"][0], (*itr)["p2"][1], (*itr)["p2"][2]),
                    new Vector3f((*itr)["p3"][0], (*itr)["p3"][1], (*itr)["p3"][2]),
                    new Vector3f((*itr)["p4"][0], (*itr)["p4"][1], (*itr)["p4"][2]),
                    new Vector3f((*itr)["id"][0], (*itr)["id"][1], (*itr)["id"][2]),
                    new Vector3f((*itr)["is"][0], (*itr)["is"][1], (*itr)["is"][2]),
                    (*itr)["n"], (*itr)["usecenter"]
                ));
            }
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

        vector<int> raysperpixel;

        if ((*itr)["raysperpixel"].size() > 0) {
            raysperpixel.push_back((int)(*itr)["raysperpixel"][0]);

            if ((*itr)["raysperpixel"].size() > 1) {
                raysperpixel.push_back((int)(*itr)["raysperpixel"][1]);

                if ((*itr)["raysperpixel"].size() == 3) {
                    raysperpixel.push_back((int)(*itr)["raysperpixel"][2]);
                }
            }
        }

        bool antialiasing = false;
        if ((*itr).contains("antialiasing")) antialiasing = (*itr)["antialiasing"];

        bool globalillum = false;
        if ((*itr).contains("globalillum")) globalillum = (*itr)["globalillum"];

        int maxbounces = 0;
        if ((*itr).contains("maxbounces")) maxbounces = (*itr)["maxbounces"];

        float probterminate = 0;
        if ((*itr).contains("probterminate")) probterminate = (*itr)["probterminate"];

        img = new Image(
            (*itr)["filename"],
            (*itr)["size"][0], (*itr)["size"][1],
            new Vector3f((*itr)["ai"][0],(*itr)["ai"][1],(*itr)["ai"][2]),
            new Vector3f((*itr)["bkc"][0],(*itr)["bkc"][1],(*itr)["bkc"][2]),
            raysperpixel,
            antialiasing,
            globalillum,
            maxbounces,
            probterminate);

        cam = new Camera(
            new Vector3f((*itr)["centre"][0], (*itr)["centre"][1], (*itr)["centre"][2]),
            new Vector3f((*itr)["lookat"][0], (*itr)["lookat"][1], (*itr)["lookat"][2]),
            new Vector3f((*itr)["up"][0], (*itr)["up"][1], (*itr)["up"][2]),
            (*itr)["fov"]);

        temp.push_back(new Output(cam, img));
    }

    return temp;
}



/* ### Ray ### */

/// Ray class constructor
/// \param origin Where the ray strats from
/// \param dir The direction of the ray
/// \param sha Shape to raycast at
Ray::Ray(Vector3f origin, Vector3f dir, Shape *sha) {
    dir = dir.normalized(); // Making sure the direction is normalized

    // Checking to see which shape we are raycasting at, so we can hit it properly
    if (sha->get_type() == "Rectangle")
        does_hit = hit_rectangle(origin, dir, dynamic_cast<Rectangle*>(sha));
    else if (sha->get_type() == "Sphere")
        does_hit = hit_sphere(origin, dir, dynamic_cast<Sphere*>(sha));
}

/// Ray destructor
Ray::~Ray() {
    if (does_hit) {
        delete hit;
        hit = NULL;

        delete hit_normal;
        hit_normal = NULL;
    }
}

/// Method to determine whether a ray is on the right of a line
/// \param ray The ray to check
/// \param p1 The first point of the line
/// \param p2 The second point of the line
/// \param n The normal of the line
/// \return Whether the ray is on the right of the line
bool is_on_right(Vector3f ray, Vector3f p1, Vector3f p2, Vector3f n) {
    return ((p1 - p2).cross(ray - p2)).dot(n) > 0;
}

/// Method to see if the ray hits a triangle
/// \param check The point to check against the triangle
/// \param tri Triangle to check against
/// \return Whether the ray is within the triangle
bool Ray::hit_triangle(Vector3f check, Triangle *tri) {
    bool ba = is_on_right(check, *tri->B(), *tri->A(), *tri->get_normal());
    bool cb = is_on_right(check, *tri->C(), *tri->B(), *tri->get_normal());
    bool ac = is_on_right(check, *tri->A(), *tri->C(), *tri->get_normal());

    // Only hits if it's on the same side as all the triangle's lines
    bool temp = (ba == cb) && (cb == ac);

    if (temp) {
        hit = new Vector3f(check);
        hit_normal = new Vector3f(*tri->get_normal());
        hit_shape = tri;
    }

    return temp;
}

/// Quick method to calculate the position where a ray intersects with a plane
/// \param plane_point Any point on the plane
/// \param plane_normal The normal of the plane
/// \param ray_origin The origin of the ray
/// \param ray_direction The direction the ray is going
/// \return Where the ray intersects the plane
Vector3f project_to_plane(
        Vector3f plane_point, Vector3f plane_normal,
        Vector3f ray_origin, Vector3f ray_direction) {
    float A = (plane_normal).dot(plane_point - ray_origin);
    float B = plane_normal.dot(ray_direction);
    float t = A / B;

    Vector3f projection = ray_origin + ray_direction * t;

    if (t > 0) return projection; // Making sure that the project is actually in front of the ray
    else throw "No projection!";
}

/// Method to see if the ray hits a rectangle
/// \param check_origin Origin of the ray
/// \param check_direction Direction the ray is going in
/// \param rect Rectangle to check against
/// \return Whether the ray hits the rectangle
bool Ray::hit_rectangle(Vector3f check_origin, Vector3f check_direction, Rectangle *rect) {
    try {
        // Trying to project to the Rectangle's plane
        Vector3f projection = project_to_plane(
            *rect->A(),
            *rect->get_normal(),
            check_origin,
            check_direction
        );

        // checking the first Triangle, then the second (if it doesn't hit)
        bool valid = hit_triangle(projection, rect->getT1());
        if (!valid) valid = hit_triangle(projection, rect->getT2());

        if (valid) hit_shape = rect;

        return valid;
    }
    catch (const char* msg) { return false; }
}

/// Method to see if the ray hits a sphere
/// \param o Origin of the ray
/// \param dir Direction the ray is going in
/// \param sph Sphere to check against
/// \return Whether the ray hits the sphere
bool Ray::hit_sphere(Vector3f o, Vector3f dir, Sphere *sph) {
    Vector3f toSphere = o - *sph->get_origin();

    // Getting the components of the quadratic formula
    float a = dir.dot(dir);
    float b = 2 * toSphere.dot(dir);
    float c = toSphere.dot(toSphere) - (sph->get_radius() * sph->get_radius());

    // Getting the critical determinant
    float determinant = sqrt((b * b) - 4 * a * c);

    // Finding the two possible roots
    float root1 = (-b + sqrt(determinant)) / (2 * a);
    float root2 = (-b - sqrt(determinant)) / (2 * a);

    if (determinant >= 0) {
        // Finding which root is closer (the hit which will be displayed)
        hit = new Vector3f(o + dir * (root1 < root2 ? root1 : root2));

        hit_normal = new Vector3f((*hit - *sph->get_origin()).normalized());
        hit_shape = sph;
    }

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

/// Free method to clamp a vector between a minimum and maximum
/// \param v Vector to clamp
/// \param min Minimum amount for the vector
/// \param max Maximum amount for the vector
/// \return The clamped vector
Vector3f clamp(Vector3f v, float min, float max) {
    v.x() = clamp(v.x(), 0, 1);
    v.y() = clamp(v.y(), 0, 1);
    v.z() = clamp(v.z(), 0, 1);

    return v;
}

/// Method to calculate the intensity for a particular raycast for a single light
/// \param hit The hit point for the raycast
/// \param sha The shape being hit
/// \param poi The light source point
/// \param all_shapes A list of all the shapes in the scene
/// \param global Whether or not the scene uses global illumination
/// \return An RGB intensity for the given raycast
Vector3f Ray::get_intensity(
        Vector3f *hit,
        Shape *sha,
        Vector3f poi,
        vector<Shape*> all_shapes,
        bool global) {
    bool local_shadow = false;

    if (!global) {
        for (Shape* s : all_shapes) {
            if (s != sha) {
                Ray *ray = new Ray(*hit, poi - *hit, s);

                local_shadow = ray->get_does_hit();

                delete ray;
                ray = NULL;

                if (local_shadow) break;
            }
        }
    }

    Vector3f N;

    // Get the respective normal
    if (sha->get_type() == "Rectangle") {
        N = -*dynamic_cast<Rectangle*>(sha)->get_normal();
    }
    else if (sha->get_type() == "Sphere") {
        N = (*hit - *dynamic_cast<Sphere*>(sha)->get_origin()).normalized();
    }

    Vector3f L = (poi - *hit).normalized(); // Direction from the hit to the light source

    float lambertian = clamp(N.dot(L), 0); // Setting up Lambert's cosine law
    float specular = 0;

    if (lambertian > 0) {
        //Vector3f R = (-L) - 2 * ((-L).dot(N)) * N;
        Vector3f V = (-*hit).normalized();
        Vector3f H = (L + V) / (L + V).norm();

        // Getting the specular
        float specAngle = clamp(H.dot(N), 0);
        specular = pow(specAngle, sha->get_phong_coefficient());
    }

    // Getting the three light/colour values
    Vector3f diff = sha->get_diffuse_coefficient() * lambertian * *sha->get_diffuse_colour();
    Vector3f spec = sha->get_specular_coefficient() * specular * *sha->get_specular_colour();

    Vector3f intensity;

    if (!global) {
        intensity = spec;

        if (!local_shadow) intensity += diff;
    }
    else {
        intensity = diff;
    }

    return intensity;
}

/// Method to get the sampled average light from the area light
/// \param hit The hit point for the raycast
/// \param sha The shape being hit
/// \param area The area light source
/// \param all_shapes A list of all the shapes in the scene
/// \param global Whether or not the scene uses global illumination
/// \return The average intensity of the area light
Vector3f Ray::get_area_intensity(
        Vector3f *hit,
        Shape *sha,
        Area *area,
        vector<Shape*> all_shapes,
        bool global) {
    Vector3f average(0, 0, 0);

    float cell_size = 1.0 / area->get_n();

    Vector3f increment_x = (*area->P2() - *area->P1()).normalized() * cell_size;
    Vector3f increment_y = (*area->P3() - *area->P1()).normalized() * cell_size;

    for (int i = 0; i < area->get_n(); i++)
        for (int j = 0; j < area->get_n(); j++)
            average += get_intensity(
                hit,
                sha,
                *area->P1() + ((i + (cell_size / 2)) * increment_x) + ((j + (cell_size / 2)) * increment_y),
                all_shapes,
                global);

    average /= pow(area->get_n(), 2);

    return average;
}

/// Gets the average intensity from all lights at a particular point
/// \param hit The hit point for the raycast
/// \param sha The shape being hit
/// \param poi The light source point
/// \param all_shapes A list of all the shapes in the scene
/// \param all_lights A list of all the lights in the scene
/// \param global Whether or not the scene uses global illumination
/// \return An average light intensity at a point
Vector3f Ray::get_average_intensity(
        Vector3f *hit,
        Shape *sha,
        vector<Shape*> all_shapes,
        vector<Light*> all_lights,
        bool global) {
    Vector3f ambient = sha->get_ambient_coefficient() * *sha->get_ambient_colour();
    Vector3f intensity(0, 0, 0); // Average intensity value (to be divided later)

    for (Light* l : all_lights) {
        if (l->get_type() == "Point") {
            intensity += get_intensity(
                hit,
                sha,
                *dynamic_cast<Point *>(l)->get_origin(),
                all_shapes,
                global);
        }
        else if (l->get_type() == "Area") {
            Area *area = dynamic_cast<Area *>(l);

            if (!global && !area->get_usecenter()) {
                intensity += get_area_intensity(hit, sha, area, all_shapes, global);
            }
            else {
                intensity += get_intensity(
                    hit,
                    sha,
                    (*area->P3() - *area->P1()) / 2,
                    all_shapes,
                    global);
            }
        }
    }

    return clamp(global ? intensity : ambient + intensity, 0, 1);
}

/// Hit point getter
/// \return The hit point of the raycast
Vector3f *Ray::get_hit() { return hit; }
/// Hit point normal getter
/// \return The normal at the hit point
Vector3f *Ray::get_hit_normal() { return hit_normal; }
/// Hit point Shape getter
/// \return The Shape that the raycast hit
Shape *Ray::get_hit_shape() { return hit_shape; }
/// Does hit getter
/// \return Whether the raycast hits the shape
bool Ray::get_does_hit() { return does_hit; }