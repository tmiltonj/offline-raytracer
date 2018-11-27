#ifndef __RAYTRACER_HPP
#define __RAYTRACER_HPP

#include <memory>

#include "objects.hpp"

typedef std::unique_ptr<Vec3[]> Pixel1D;
typedef std::unique_ptr<Pixel1D[]> Pixel2D;

Pixel2D raytrace(std::shared_ptr<Scene> scene, int &width, int &height, 
                    int recursion_level = 0, int ssample_level = 1);

Collision fire_ray(Vec3 p0, Vec3 d, std::shared_ptr<Scene> scene);

Vec3 compute_color(Collision col, std::shared_ptr<Scene> scene, Vec3 view_pos, int rec_depth);
Vec3 calc_phong(std::shared_ptr<Light> light, std::shared_ptr<Object> obj, Vec3 pos, Vec3 view_pos);

#endif