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
                true, false, true,
                shapes);

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
        }

        if (type == "triangle") {
            cout << "TRIANGLE" << endl;

            temp.push_back(new Triangle(
                new Vector3<float>((*itr)["p1"][0], (*itr)["p1"][1], (*itr)["p1"][2]),
                new Vector3<float>((*itr)["p2"][0], (*itr)["p2"][1], (*itr)["p2"][2]),
                new Vector3<float>((*itr)["p3"][0], (*itr)["p3"][1], (*itr)["p3"][2]),
                new Vector3<float>((*itr)["ac"][0], (*itr)["ac"][1], (*itr)["ac"][2]),
                new Vector3<float>((*itr)["dc"][0], (*itr)["dc"][1], (*itr)["dc"][2]),
                new Vector3<float>((*itr)["sc"][0], (*itr)["sc"][1], (*itr)["sc"][2]),
                (*itr)["ka"], (*itr)["kd"], (*itr)["ks"],
                (*itr)["pc"],
                NULL,
                false
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
        else if (type == "sphere") {
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
                new Vector3<float>((*itr)["is"][0], (*itr)["is"][1], (*itr)["is"][2]),
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

        img = new Image(
            (*itr)["filename"],
            (*itr)["size"][0], (*itr)["size"][1],
            new Vector3<float>((*itr)["ai"][0],(*itr)["ai"][1],(*itr)["ai"][2]),
            new Vector3<float>((*itr)["bkc"][0],(*itr)["bkc"][1],(*itr)["bkc"][2]),
            raysperpixel,
            antialiasing,
            globalillum);

        cam = new Camera(
            new Vector3<float>((*itr)["centre"][0], (*itr)["centre"][1], (*itr)["centre"][2]),
            new Vector3<float>((*itr)["lookat"][0], (*itr)["lookat"][1], (*itr)["lookat"][2]),
            new Vector3<float>((*itr)["up"][0], (*itr)["up"][1], (*itr)["up"][2]),
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
Ray::Ray(Image *img, Camera *cam, Vector3<float> base, Shape *sha, Vector2<float> indices, Vector2<float> offset) {
    float pixel_size = img->get_pixel_size(cam);

    Vector3<float> out =
        base +
        (*cam->get_up() * ((-indices[1] + offset[1]) * pixel_size + (pixel_size / 2))) +
        (*cam->get_side() * ((indices[0] + offset[0]) * pixel_size + (pixel_size / 2)));

    // Checking to see which shape we are raycasting at, so we can hit it properly
    if (sha->getType() == "Triangle") {
        Triangle *tri = dynamic_cast<Triangle*>(sha);

        raycast = new Vector3<float>(get_triangle_raycast(cam->get_origin(), tri->get_normal(), out));

        does_hit = hit_triangle(*raycast, tri, true);
    }
    if (sha->getType() == "Rectangle") {
        Rectangle *rec = dynamic_cast<Rectangle*>(sha);

        raycast = new Vector3<float>(get_triangle_raycast(cam->get_origin(), rec->getT1()->get_normal(), out));
        does_hit = hit_triangle(*raycast, rec->getT1(), true);

        if (!does_hit) {
            delete raycast;
            raycast = NULL;

            raycast = new Vector3<float>(get_triangle_raycast(cam->get_origin(), rec->getT2()->get_normal(), out));
            does_hit = hit_triangle(*raycast, rec->getT2(), true);
        }
    }
    else if (sha->getType() == "Sphere") {
        raycast = new Vector3<float>(out);

        Sphere *sph = dynamic_cast<Sphere*>(sha);

        does_hit = hit_sphere(*cam->get_origin(), raycast->normalized(), sph, true);
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
bool is_on_right(Vector3<float> ray, Vector3<float> p1, Vector3<float> p2, Vector3<float> n) {
    return ((p1 - p2).cross(ray - p2)).dot(n) > 0;
}

/// Method to see if the raycast hits a triangle
/// \param tri Triangle to hit
/// \return Whether the ray does hit
bool Ray::hit_triangle(Vector3<float> check, Triangle *tri, bool update_hit) {
    bool ba = is_on_right(check, *tri->B(), *tri->A(), *tri->get_normal());
    bool cb = is_on_right(check, *tri->C(), *tri->B(), *tri->get_normal());
    bool ac = is_on_right(check, *tri->A(), *tri->C(), *tri->get_normal());

    // Only hits if it's on the same side as all the triangle's lines
    bool temp = (ba == cb) && (cb == ac);

    if (temp && update_hit) hit = raycast;

    return temp;
}

/// Method to see if the raycast hits a sphere
/// \param o Origin of the raycast
/// \param sph Sphere to hit
/// \return Whether the ray does hit
bool Ray::hit_sphere(Vector3<float> o, Vector3<float> dir, Sphere *sph, bool update_hit) {
    Vector3<float> toSphere = o - *sph->get_origin();

    // Getting the components of the quadratic formula
    float a = dir.dot(dir);
    float b = 2 * toSphere.dot(dir);
    float c = toSphere.dot(toSphere) - (sph->get_radius() * sph->get_radius());

    // Getting the critical determinant
    float determinant = sqrt((b * b) - 4 * a * c);

    // Finding the two possible roots
    float root1 = (-b + sqrt(determinant)) / (2 * a);
    float root2 = (-b - sqrt(determinant)) / (2 * a);

    if (update_hit) {
        // Finding which root is closer (the hit which will be displayed)
        hit = new Vector3<float>(o + dir * (root1 < root2 ? root1 : root2));
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

/// Method to calculate the intensity for a particular raycast
/// \param hit The hit point for the raycast
/// \param sha The shape being hit
/// \param poi The light source point
/// \param shininess The amount of shine
/// \return An RGB intensity for the given raycast
Vector3<float> Ray::get_intensity(
        Vector3<float> *hit,
        Shape *sha,
        Vector3<float> poi,
        float shininess,
        vector<Shape*> all_shapes) {
    bool hits = false;

    for (Shape* i : all_shapes) {
        if (i != sha) {
            // TODO: do proper local illumination shadows

            /*
            if (i->getType() == "Triangle") {
                Triangle* temp = dynamic_cast<Triangle*>(i);
                hits = hit_triangle(get_triangle_raycast(hit, temp->get_normal(), poi - *hit), temp);
            }
            else if (i->getType() == "Rectangle") {
                Rectangle* temp = dynamic_cast<Rectangle*>(i);

                hits = hit_triangle(
                        get_triangle_raycast(hit, temp->get_normal(), poi - *hit),
                        temp->getT1()
                );

                if (!hits)
                    hits = hit_triangle(
                            get_triangle_raycast(hit, temp->get_normal(), poi - *hit),
                            temp->getT2()
                    );
            }
            else if (i->getType() == "Sphere") {
                hits = hit_sphere(*hit, (poi - *hit).normalized(), dynamic_cast<Sphere*>(i), false);
            }
            */

            if (hits) return { 0, 0, 0 };
        }
    }

    Vector3<float> N;

    // Get the respective normal
    if (sha->getType() == "Triangle") {
        N = -*dynamic_cast<Triangle*>(sha)->get_normal();
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

/// Hit point getter
/// \return The hit point of the raycast
Vector3<float> *Ray::get_hit() { return hit; }
/// Raycast vector getter
/// \return The vector of the raycast
Vector3<float> *Ray::get_raycast() { return raycast; }
/// Does hit getter
/// \return Whether the raycast hits the shape
bool Ray::get_does_hit() { return does_hit; }