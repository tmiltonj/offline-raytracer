#ifndef __RAYTRACER_HPP
#define __RAYTRACER_HPP

#include <memory>

#include "objects.hpp"

// Type for our image data (2d array of Vec3)
typedef std::unique_ptr<Vec3[]> Pixel1D;
typedef std::unique_ptr<Pixel1D[]> Pixel2D;


/* Raytrace
 * Main raytracing function
 * Calculates pixel colours of an image in the range [0.0, 1.0] using backwards raytracing
 *
 * PARAMETERS 
 * scene - The scene to raytrace
 * width, height - Will store the dimensions of the image since they are calculated based on camera's 
 *                  focal length, fov and aspect ratio
 * 
 * OPTIONAL
 * recursion_level - How many recursive reflections to render (default 0)
 * ssample_div - Supersampling level, each pixel will be an average of ssample_div^2 rays (default 1)
 * num_shadows - How many rays to fire for soft shadows (default 1)
 */
Pixel2D raytrace(std::shared_ptr<Scene> scene, int &width, int &height, 
                    int recursion_level = 0, int ssample_level = 1, int num_shadows = 1);


/* Checks if a ray collides with an object in the scene
 * Returns the object and position of the collision if the ray collides
 * Returns NO_COLLISION otherwise
 */
Collision fire_ray(Vec3 p0, Vec3 d, std::shared_ptr<Scene> scene);


Vec3 compute_color(Collision col, std::shared_ptr<Scene> scene, Vec3 view_pos, int rec_depth, int num_shadows);
Vec3 calc_phong(std::shared_ptr<Light> light, std::shared_ptr<Object> obj, Vec3 pos, Vec3 view_pos);

#endif