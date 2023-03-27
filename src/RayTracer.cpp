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
                o->get_image(),
                s,
                lights,
                false, true,
                shapes);

            cout << "Rendered: " << s->getType() << endl;
        }

        cout << endl << "[" << o->get_image()->get_name() << "] number of hits: " << o->get_image()->get_number_of_hits() << endl << endl;
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

/// Gets the raycast hit point for a triangle
/// \param o Origin of the raycast
/// \param n Normal of the triangle
/// \param out Camera-to-image-plane outgoing vector
/// \return The 3D hit point of the triangle raycast
Vector3f triangle_intersect(Vector3f o, Vector3f n, Vector3f out) {
    float t = -(o.dot(n) - 4) / (out.dot(n));
    return o + (out * t);
}

/// Ray class constructor
/// \param sha Shape to raycast at
Ray::Ray(Vector3f origin, Vector3f ray, Shape *sha, bool global) {
    Vector3f dir = ray.normalized();

    // Checking to see which shape we are raycasting at, so we can hit it properly
    if (sha->getType() == "Rectangle") {
        Rectangle *rec = dynamic_cast<Rectangle*>(sha);

        float t;

        if (global) {
            t = (*rec->A() - origin).dot(*rec->get_normal()) / dir.dot(*rec->get_normal());
            raycast = new Vector3f(origin + t * dir);
        }
        else {
            raycast = new Vector3f(triangle_intersect(origin, *rec->getT1()->get_normal(), ray));
        }

        does_hit = hit_triangle(*raycast, rec->getT1());

        if (!does_hit) does_hit = hit_triangle(*raycast, rec->getT2());
    }
    else if (sha->getType() == "Sphere") {
        raycast = new Vector3f(ray);

        does_hit = hit_sphere(
            origin,
            dir,
            dynamic_cast<Sphere*>(sha)
        );
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
bool is_on_right(Vector3f ray, Vector3f p1, Vector3f p2, Vector3f n) {
    return ((p1 - p2).cross(ray - p2)).dot(n) > 0;
}

/// Method to see if the raycast hits a triangle
/// \param tri Triangle to hit
/// \return Whether the ray does hit
bool Ray::hit_triangle(Vector3f check, Triangle *tri) {
    bool ba = is_on_right(check, *tri->B(), *tri->A(), *tri->get_normal());
    bool cb = is_on_right(check, *tri->C(), *tri->B(), *tri->get_normal());
    bool ac = is_on_right(check, *tri->A(), *tri->C(), *tri->get_normal());

    // Only hits if it's on the same side as all the triangle's lines
    bool temp = (ba == cb) && (cb == ac);

    if (temp) hit = raycast;

    return temp;
}

/// Method to see if the raycast hits a sphere
/// \param o Origin of the raycast
/// \param sph Sphere to hit
/// \return Whether the ray does hit
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

    // Finding which root is closer (the hit which will be displayed)
    hit = new Vector3f(o + dir * (root1 < root2 ? root1 : root2));

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
Vector3f Ray::get_intensity(
        Vector3f *hit,
        Shape *sha,
        Vector3f poi,
        float shininess,
        vector<Shape*> all_shapes) {
    /*
    bool hits = false;

    for (Shape* i : all_shapes) {
        if (i != sha) {
            // TODO: do proper local illumination shadows

            if (i->getType() == "Rectangle") {

            }
            else if (i->getType() == "Sphere") {

            }

            //if (hits) cout << sha->getType() << endl;
            if (hits) return Vector3f(0, 0, 0);
        }
    }
    */

    Vector3f N;

    // Get the respective normal
    if (sha->getType() == "Rectangle") {
        N = -*dynamic_cast<Rectangle*>(sha)->get_normal();
    }
    else if (sha->getType() == "Sphere") {
        N = (*hit - *dynamic_cast<Sphere*>(sha)->get_origin()).normalized();
    }

    Vector3f L = (poi - *hit).normalized(); // Direction from the hit to the light source

    float lambertian = clamp(N.dot(L), 0); // Setting up Lambert's cosine law
    float specular;

    if (lambertian > 0) {
        Vector3f R = (-L) - 2 * ((-L).dot(N)) * N;
        Vector3f V = (-*hit).normalized();
        Vector3f H = (L + V) / (L + V).norm();

        // Getting the specular
        float specAngle = clamp(H.dot(N), 0);
        specular = pow(specAngle, shininess);
    }

    // Getting the three light/colour values
    Vector3f amb = sha->get_ambient_coefficient() * *sha->get_ambient_colour();
    Vector3f diff = sha->get_diffuse_coefficient() * lambertian * *sha->get_diffuse_colour();
    Vector3f spec = sha->get_specular_coefficient() * specular * *sha->get_specular_colour();

    Vector3f intensity = amb + diff + spec;

    // Clamping each intensity value between 0 and 1
    intensity.x() = clamp(intensity.x(), 0, 1);
    intensity.y() = clamp(intensity.y(), 0, 1);
    intensity.z() = clamp(intensity.z(), 0, 1);

    return intensity;
}

/// Hit point getter
/// \return The hit point of the raycast
Vector3f *Ray::get_hit() { return hit; }
/// Raycast vector getter
/// \return The vector of the raycast
Vector3f *Ray::get_raycast() { return raycast; }
/// Does hit getter
/// \return Whether the raycast hits the shape
bool Ray::get_does_hit() { return does_hit; }