#ifndef __OBJECTS_HPP
#define __OBJECTS_HPP

#include <stdexcept>

#include <glm/glm.hpp>

#include "raytracer.hpp"

class Object
{
public:
    virtual Vector get_normal(Coord point) = 0;
    virtual float check_collision(Coord p0, Vector d) = 0;

    virtual ~Object() {};
    Object(glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shi);

private:
    glm::vec3 amb, dif, spe;
    float shi;
};


class Plane : public Object
{
public:
    Plane(Vector normal, Coord point, 
            glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shi);

    Vector get_normal(Coord point) override;
    float check_collision(Coord p0, Vector d) override;

private:
    Vector normal;
    Coord point;
};


class Sphere : public Object
{
public:
    Sphere(Coord pos, float r,
            glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shi);

    Vector get_normal(Coord point) override;
    float check_collision(Coord p0, Vector d) override;

private:
    float r;
    Coord pos;
};

/*
class Mesh : public Object
{

};

class Light
{
    
};
*/

#endif