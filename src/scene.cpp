#include "scene.hpp"

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

Scene::Scene()
{
    camera = nullptr;
    objects = std::vector<std::shared_ptr<Object>>();
}

/*
Scene::~Scene()
{
    camera = nullptr;
    for (int i = 0; i < objects.size(); i++) {
        objects[i] = nullptr;
    }
}
*/

std::shared_ptr<Scene> load_scene(const char *filename)
{
    std::shared_ptr<Scene> scene(std::make_shared<Scene>());

    return scene;
}