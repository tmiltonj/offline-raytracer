#ifndef __OBJECTS_HPP
#define __OBJECTS_HPP

#include <cstdio>
#include <memory>
#include <vector>
#include <stdexcept>

#include <glm/glm.hpp>


typedef glm::vec3 Vec3;
typedef glm::vec3 Coord;

class Object;

typedef struct
{
    std::weak_ptr<Object> obj;
    Coord coord;
} Collision;



class Camera
{
public:
    Coord pos;
    int fov, f;
    float a;

    Camera(Coord pos, int fov, int f, float a);
};



bool valid_light(Vec3 L);

class Light
{
public:
    Coord pos;
    Vec3 amb, dif, spe;

    Light(Coord pos, Vec3 amb, Vec3 dif, Vec3 spe);
};



class Object
{
public:
    virtual Vec3 get_normal(Coord point) = 0;
    virtual float check_collision(Coord p0, Vec3 d) = 0;

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
    std::vector<std::shared_ptr<Light>> lights;

    Scene();
};



class Plane : public Object
{
public:
    Plane(Vec3 normal, Coord point, 
            glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shi);

    Vec3 get_normal(Coord point) override;
    float check_collision(Coord p0, Vec3 d) override;

private:
    Vec3 normal;
    Coord point;
};



class Sphere : public Object
{
public:
    Sphere(Coord pos, float r,
            glm::vec3 amb, glm::vec3 dif, glm::vec3 spe, float shi);

    Vec3 get_normal(Coord point) override;
    float check_collision(Coord p0, Vec3 d) override;

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