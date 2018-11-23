#ifndef __OBJECTS_HPP
#define __OBJECTS_HPP

#include <cstdio>
#include <memory>
#include <vector>
#include <stdexcept>

#include <glm/glm.hpp>

#include "raytracer.hpp"



class Camera
{
public:
    Coord pos;
    int fov, f;
    float a;

    Camera(Coord pos, int fov, int f, float a);
};



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



class Scene
{
public:
    std::shared_ptr<Camera> camera;
    std::vector<std::shared_ptr<Object>> objects;
    //std::vector<std::shared_ptr<Light>> lights;

    Scene();
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


// FILE I/O FUNCTIONS
std::shared_ptr<Scene> load_scene(const char *filename);

Camera *parse_camera(FILE *file);
//Plane *parse_plane(FILE* file);
//Sphere *parse_sphere(FILE *file);
//Mesh *parse_mesh(FILE *file);
//Light *parse_light(FILE *file);


#endif