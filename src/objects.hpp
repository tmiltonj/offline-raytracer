#ifndef __OBJECTS_HPP
#define __OBJECTS_HPP

#include <cstdio>
#include <memory>
#include <vector>
#include <stdexcept>

#include <glm/glm.hpp>


typedef glm::vec3 Vec3;

class Object;

typedef struct
{
    std::shared_ptr<Object> obj;
    Vec3 coord;
} Collision;

const Collision NO_COLLISION { nullptr, Vec3 { 0.0 } };



class Camera
{
public:
    Vec3 pos;
    int fov, f;
    float a;

    Camera(Vec3 pos, int fov, int f, float a);
};



bool valid_light(Vec3 L);

class Light
{
public:
    Vec3 pos;
    Vec3 amb, dif, spe;

    Light(Vec3 pos, Vec3 amb, Vec3 dif, Vec3 spe);
};



class Object
{
public:
    virtual Vec3 get_normal(Vec3 point) = 0;
    virtual float check_collision(Vec3 p0, Vec3 d) = 0;

    virtual ~Object() {};
    Object(Vec3 amb, Vec3 dif, Vec3 spe, float shi);

    Vec3 amb, dif, spe;
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
    Plane(Vec3 normal, Vec3 point, 
            Vec3 amb, Vec3 dif, Vec3 spe, float shi);

    Vec3 get_normal(Vec3 point) override;
    float check_collision(Vec3 p0, Vec3 d) override;

private:
    Vec3 normal;
    Vec3 point;
};



class Sphere : public Object
{
public:
    Sphere(Vec3 pos, float r,
            Vec3 amb, Vec3 dif, Vec3 spe, float shi);

    Vec3 get_normal(Vec3 point) override;
    float check_collision(Vec3 p0, Vec3 d) override;

private:
    float r;
    Vec3 pos;
};


/*
class Mesh : public Object
{

};
*/


#endif