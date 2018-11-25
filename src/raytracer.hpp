#ifndef __RAYTRACER_HPP
#define __RAYTRACER_HPP

#include <memory>

#include "objects.hpp"

typedef std::unique_ptr<float[]> Float1D;
typedef std::unique_ptr<Float1D[]> Float2D;

Float2D raytrace(std::shared_ptr<Scene> scene, int &width, int &height);

Collision fire_ray(Vec3 p0, Vec3 d, std::shared_ptr<Scene> scene);

#endif