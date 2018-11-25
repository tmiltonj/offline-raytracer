#include "objects.hpp"

Scene::Scene()
{
    camera = nullptr;
    objects = std::vector<std::shared_ptr<Object>>();
}



Camera::Camera(Vec3 pos, int fov, int f, float a)
{
    if (fov <= 0)
        throw std::invalid_argument("fov must be > 0");

    if (f <= 0)
        throw std::invalid_argument("f must be > 0");

    if (a <= 0.0)
        throw std::invalid_argument("a must be > 0.0");

    this->pos = pos;
    this->fov = fov;
    this->f = f;
    this->a = a;
}



bool valid_light(Vec3 L)
{
    return (L.x >= 0.0 && L.y >= 0.0 && L.z >= 0.0);
}



Light::Light(Vec3 pos, Vec3 amb, Vec3 dif, Vec3 spe)
{
    if (!valid_light(amb))
        throw std::invalid_argument("Values of amb must be >= 0");

    if (!valid_light(dif))
        throw std::invalid_argument("Values of dif must be >= 0");

    if (!valid_light(spe))
        throw std::invalid_argument("Values of spe must be >= 0");

    this->pos = pos;
    this->amb = amb;
    this->dif = dif;
    this->spe = spe;
}



Object::Object(Vec3 amb, Vec3 dif, Vec3 spe, float shi)
{
    if (!valid_light(amb))
        throw std::invalid_argument("Values of amb must be >= 0");

    if (!valid_light(dif))
        throw std::invalid_argument("Values of dif must be >= 0");

    if (!valid_light(spe))
        throw std::invalid_argument("Values of spe must be >= 0");

    if (shi <= 0)
        throw std::invalid_argument("Value for shi must be >= 0.0");

    this->amb = amb;
    this->dif = dif;
    this->spe = spe;
    this->shi = shi;
}



Plane::Plane(
    Vec3 normal, Vec3 point,
    Vec3 amb, Vec3 dif, Vec3 spe, float shi) :
    Object::Object(amb, dif, spe, shi)
{
    this->normal = normal;
    this->point = point;
}

Vec3 Plane::get_normal(Vec3 point){ return normal; }

// http://www.geomalgorithms.com/a05-_intersect-1.html
float Plane::check_collision(Vec3 p0, Vec3 d)
{
    // TODO: Replace -1 w/ constant
    // if n.d = 0, the ray is perpendicular to the plane
    if (glm::dot(normal, d) == 0){ return -1; }

    Vec3 p1 { p0 + d };
    return { glm::dot(normal, point - p0) / glm::dot(normal, p1 - p0) };
}



Sphere::Sphere(
    Vec3 pos, float r,
    Vec3 amb, Vec3 dif, Vec3 spe, float shi) :
    Object::Object(amb, dif, spe, shi)
{
    if (r <= 0.0)
        throw std::invalid_argument("r must be >0.0");

    this->pos = pos;
    this->r = r;
}


Vec3 Sphere::get_normal(Vec3 point)
{
    return (point != pos) ? glm::normalize(point - pos) : Vec3 { 0.0 };
}


float Sphere::check_collision(Vec3 p0, Vec3 d)
{
    Vec3 p_dif { p0 - pos };
    float a, b, c, radicand;
    a = 1.0;
    b = 2.0 * (d.x * p_dif.x + d.y * p_dif.y + d.z * p_dif.z);
    c = pow(p_dif.x, 2.0) + pow(p_dif.y, 2.0) + pow(p_dif.z, 2.0) - pow(r, 2.0);

    float t, t0, t1, sqrt_rad;
    radicand = pow(b, 2) - (4.0 * a * c);
    if (radicand < 0)
    {
        t = -1.0; // TODO: Replace with a constant
    }
    else
    {
        sqrt_rad = sqrt(radicand);
        t0 = (-b + sqrt_rad) / 2.0;
        t1 = (-b - sqrt_rad) / 2.0;

        if (t0 < 0) {
            // t0 is behind the ray
            t = t1;
        }
        else if (t1 < 0) {
            // t1 is behind the ray
            t = t0;
        }
        else {
            // Both are in front, find the closest
            t = fmin(t0, t1);
        }
    }

    return t;
}


std::shared_ptr<Scene> load_scene(const char *filename)
{
    std::shared_ptr<Scene> scene(std::make_shared<Scene>());

    return scene;
}

