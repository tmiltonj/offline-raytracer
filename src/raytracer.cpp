#include <limits>
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/constants.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "raytracer.hpp"



const Vec3 BACKGROUND_COLOUR { 0.0 };

// Controls how reflective surfaces are when recursion_level > 0
const float SPECULARITY { 0.3 };

// How far to spread rays when calculating soft shadows
const float AREA_LIGHT_OFFSET { 5 };

// Reduces shadow acne caused by floating-point precision errors
const float BIAS { 0.1f };



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
                    int recursion_level, int ssample_div, int num_shadows)
{
    std::shared_ptr<Camera> cam { scene->camera };
    Vec3 cam_pos { cam->pos };
    int cam_f { cam->f };
    float fov_r { glm::radians((float)(cam->fov)) };

    // Calculate image size
    height = ceil(2.0 * cam->f * tan(fov_r / 2.0));
    width = ceil(cam->a * height);

    // Initialize pixels
    Pixel2D px_data { new Pixel1D[width] };
    for (int i = 0; i < width; i++)
        px_data[i] = Pixel1D(new Vec3[height]);

    // Calculate level of supersampling
    ssample_div = (ssample_div < 1) ? 1 : ssample_div;
    float ssample_step { 1.0f / ssample_div };

    // Fire ray for each pixel
    Collision col;
    Vec3 px_screen_space, px_world_space, px_offset, ray_dir;
    px_offset = Vec3 { width / 2, -height / 2, 0 };

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            px_data[x][y] = Vec3 { 0.0 };
            // Supersampling loop
            for (int i = 0; i < ssample_div; i++)
            {
                for (int j = 0; j < ssample_div; j++)
                {
                    // Compute pixel location in world space
                    px_screen_space = Vec3 { x + i * ssample_step, -y + j * ssample_step, -cam_f };
                    px_world_space = px_screen_space - px_offset;
                    ray_dir = glm::normalize(px_world_space - cam_pos);

                    // Check for collision
                    col = fire_ray(cam_pos, ray_dir, scene);

                    if (col == NO_COLLISION) {
                        px_data[x][y] += BACKGROUND_COLOUR;
                    }
                    else {
                        px_data[x][y] += compute_color(col, scene, cam_pos, recursion_level, num_shadows);
                    }
                }
            }

            // Average to account for supersampling
            px_data[x][y] = px_data[x][y] / (float)(ssample_div * ssample_div);
        }
    }

    return px_data;
}



/* Checks if a ray collides with an object in the scene
 * Returns the object and position of the collision if the ray collides
 * Returns NO_COLLISION otherwise
 */
Collision fire_ray(Vec3 p0, Vec3 d, std::shared_ptr<Scene> scene)
{
    float t, t_candidate;
    t = std::numeric_limits<float>::infinity();
    std::shared_ptr<Object> obj, next_obj;
    obj =  nullptr;

    // Test collision against every object in scene
    for (unsigned int i = 0; i < scene->objects.size(); i++)
    {
        next_obj = scene->objects[i];
        t_candidate = next_obj->check_collision(p0, d);
        if (t_candidate - BIAS > 0.0 && t_candidate < t && (t_candidate - t) < BIAS)
        {
            t = t_candidate;
            obj = next_obj;
        }
    }

    if (t < std::numeric_limits<float>::infinity())
    {
        Vec3 p_col { p0 + d * t };
        return Collision { obj, p_col };           
    }
    else
    {
        return NO_COLLISION;
    }
}



/* compute_color
 * Computes color at a given point with a given object's properties
 * 
 */
Vec3 compute_color(Collision col, std::shared_ptr<Scene> scene, Vec3 view_pos, int rec_depth, int num_rays)
{
    Vec3 normal, color, l, temp_l, phong;
    normal = glm::normalize(col.obj->get_normal(col.coord));
    color = Vec3 { 0.0 };
    
    std::shared_ptr<Light> light;
    Collision shadow_col;

    // Calculate contribution from each light source
    for (unsigned int i = 0; i < scene->lights.size(); i++)
    {
        light = scene->lights[i];
        l = light->pos - col.coord;

        // Scattering for soft shadows
        float rotation, mag;
        rotation = 2.0f * glm::pi<float>() / num_rays ;
        mag = AREA_LIGHT_OFFSET / num_rays;
        Vec3 offset { glm::normalize(glm::cross(l, normal)) * mag };
        int in_shadow { num_rays };
        
        // Fire multiple rays to points near light and average the result to determine how in shadow a point is
        for (int j = 0; j < num_rays; j++)
        {
            temp_l = light->pos + offset - col.coord;
            shadow_col = fire_ray(col.coord, glm::normalize(temp_l), scene);
            if (shadow_col == NO_COLLISION || glm::length(l) < glm::length(col.coord - shadow_col.coord)){
                in_shadow--;
            }

            offset = glm::normalize(glm::rotate(offset, rotation + j, l)) * ((i % (j + 1) + 1) * mag);
        }

        // Lights always contribute their ambient amount
        color += light->amb * col.obj->amb;

        // The ray is not completely in shadow
        if (in_shadow < num_rays)
        {   
            // Phong illumination
            phong = calc_phong(light, col.obj, col.coord, view_pos);

            // Specular reflection
            Vec3 r, specular_ref;
            specular_ref = Vec3 { 0.0 };
            if (rec_depth > 0)
            {
                r = Vec3 { glm::reflect(l, normal) };                
                Collision spec_col { fire_ray(col.coord, r, scene) };
                if (spec_col == NO_COLLISION)
                {
                    specular_ref = (specular_ref == Vec3{ 0.0 }) ? BACKGROUND_COLOUR : specular_ref;
                }
                else
                {
                    // Don't compute soft shadows when firing recursive rays
                    specular_ref = compute_color(spec_col, scene, col.coord, rec_depth - 1, 1);
                }
            }
            
            // Attenuate by amount point is in shadow
            float shadow_amount { (float)in_shadow / (float)num_rays };
            color += (1 - shadow_amount) * (phong + (SPECULARITY * col.obj->spe * specular_ref));
        }
    }

    return Vec3 { fmin(color.x, 1.0), fmin(color.y, 1.0), fmin(color.z, 1.0) };
}



// Calculate Phong illumination at a given point
Vec3 calc_phong(std::shared_ptr<Light> light, std::shared_ptr<Object> obj, Vec3 pos, Vec3 view_pos)
{
    Vec3 l, n, v, r;
    l = glm::normalize(light->pos - pos);
    n = glm::normalize(obj->get_normal(pos));
    v = glm::normalize(view_pos - pos);
    r = glm::reflect(l, n);

    float l_angle, v_angle;
    l_angle = fmax(glm::dot(l, n), 0.0);
    v_angle = fmax(glm::dot(r, v), 0.0);

    Vec3 amb, dif, spe;
    amb = light->amb * obj->amb;
    dif = light->dif * obj->dif * l_angle;
    spe = light->spe * obj->spe * (float)pow(v_angle, obj->shi);

    // Ignore ambient amount because we are adding it in compute_color
    return (dif + spe);
}