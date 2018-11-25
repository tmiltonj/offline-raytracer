#ifndef __RAYTRACER_HPP
#define __RAYTRACER_HPP

#include <memory>

#include "objects.hpp"

typedef std::unique_ptr<Vec3[]> Float1D;
typedef std::unique_ptr<Float1D[]> Float2D;

Float2D raytrace(std::shared_ptr<Scene> scene, int &width, int &height);

Collision fire_ray(Vec3 p0, Vec3 d, std::shared_ptr<Scene> scene);

Vec3 compute_color(Collision col, std::shared_ptr<Scene> scene);
Vec3 calc_phong(std::shared_ptr<Light> light, std::shared_ptr<Object> obj, Vec3 pos);

#endif