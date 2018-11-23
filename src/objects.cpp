#include "objects.hpp"

Scene::Scene()
{
    camera = nullptr;
    objects = std::vector<std::shared_ptr<Object>>();
}



Camera::Camera(Coord pos, int fov, int f, float a)
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



Light::Light(Coord pos, Vec3 amb, Vec3 dif, Vec3 spe)
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



Object::Object(glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shi)
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
    Vec3 normal, Coord point,
    glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shi) :
    Object::Object(amb, dif, spe, shi)
{
    this->normal = normal;
    this->point = point;
}

Vec3 Plane::get_normal(Coord point){ return normal; }
float Plane::check_collision(Coord p0, Vec3 d)
{
    // To be implemented
    return 1.0;
}



Sphere::Sphere(
    Coord pos, float r,
    glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shi) :
    Object::Object(amb, dif, spe, shi)
{
    if (r <= 0.0)
        throw std::invalid_argument("r must be >0.0");

    this->pos = pos;
    this->r = r;
}

Vec3 Sphere::get_normal(Coord point)
{
    // To be implemented
    return Vec3 { 1.0 };
}

float Sphere::check_collision(Coord p0, Vec3 d)
{
    // To be implemented
    return 1.0;
}


std::shared_ptr<Scene> load_scene(const char *filename)
{
    std::shared_ptr<Scene> scene(std::make_shared<Scene>());

    return scene;
}

