#ifndef __SCENE_HPP
#define __SCENE_HPP

#include <cstdio>
#include <memory>
#include <vector>

#include "raytracer.hpp"
#include "objects.hpp"

class Camera
{
public:
    Coord pos;
    int fov, f;
    float a;

    Camera(Coord pos, int fov, int f, float a);
};

class Scene
{
public:
    std::shared_ptr<Camera> camera;
    std::vector<std::shared_ptr<Object>> objects;
    //std::vector<std::shared_ptr<Light>> lights;

    Scene();
    //~Scene();
};

std::shared_ptr<Scene> load_scene(const char *filename);

Camera *parse_camera(FILE *file);
Plane *parse_plane(FILE* file);
//Sphere *parse_sphere(FILE *file);
//Mesh *parse_mesh(FILE *file);
//Light *parse_light(FILE *file);

#endif