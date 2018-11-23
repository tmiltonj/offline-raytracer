#include "objects.hpp"

Object::Object(glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shi)
{
    if (amb.x < 0 || amb.y < 0 || amb.z < 0)
        throw std::invalid_argument("Values of amb must be >= 0");

    if (dif.x < 0 || dif.y < 0 || dif.z < 0)
        throw std::invalid_argument("Values of dif must be >= 0");

    if (spe.x < 0 || spe.y < 0 || spe.z < 0)
        throw std::invalid_argument("Values of spe must be >= 0");

    if (shi <= 0)
        throw std::invalid_argument("Value for shi must be >= 0.0");

    this->amb = amb;
    this->dif = dif;
    this->spe = spe;
    this->shi = shi;
}

Plane::Plane(
    Vector normal, Coord point,
    glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shi) :
    Object::Object(amb, dif, spe, shi)
{
    this->normal = normal;
    this->point = point;
}

Vector Plane::get_normal(Coord point){ return normal; }
float Plane::check_collision(Coord p0, Vector d)
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

Vector Sphere::get_normal(Coord point)
{
    // To be implemented
    return Vector { 1.0 };
}

float Sphere::check_collision(Coord p0, Vector d)
{
    // To be implemented
    return 1.0;
}

