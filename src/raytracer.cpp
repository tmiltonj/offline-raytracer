#include <limits>
#include <memory>

#include "raytracer.hpp"


const Vec3 BACKGROUND_COLOUR { 0.0 };
const float GLOBAL_AMBIENCE { 0.2 };
const float SPECULARITY { 0.3 };

const float BIAS { 0.1f };


Pixel2D raytrace(std::shared_ptr<Scene> scene, int &width, int &height, 
                    int recursion_level, int ssample_div)
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

    // Fire ray for each pixel
    Collision col;
    Vec3 px_screen_space, px_world_space, px_offset, ray_dir;
    px_offset = Vec3 { width / 2, -height / 2, 0 };
    
    ssample_div = (ssample_div < 1) ? 1 : ssample_div;
    float ssample_step { 1.0f / ssample_div };

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            px_data[x][y] = Vec3 { 0.0 };
            for (int i = 0; i < ssample_div; i++)
            {
                for (int j = 0; j < ssample_div; j++)
                {
                    px_screen_space = Vec3 { x + i * ssample_step, -y + j * ssample_step, -cam_f };
                    px_world_space = px_screen_space - px_offset;
                    ray_dir = glm::normalize(px_world_space - cam_pos);

                    col = fire_ray(cam_pos, ray_dir, scene);

                    if (col == NO_COLLISION) {
                        px_data[x][y] += BACKGROUND_COLOUR;
                    }
                    else {
                        px_data[x][y] += compute_color(col, scene, cam_pos, recursion_level);
                    }
                }
            }
            px_data[x][y] = px_data[x][y] / (float)(ssample_div * ssample_div);
        }
    }

    return px_data;
}



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


Vec3 compute_color(Collision col, std::shared_ptr<Scene> scene, Vec3 view_pos, int rec_depth)
{
    Vec3 color, l, phong;
    color = Vec3 { 0.0 };
    //color = GLOBAL_AMBIENCE * col.obj->amb;
    
    std::shared_ptr<Light> light;
    Collision shadow_col;
    for (unsigned int i = 0; i < scene->lights.size(); i++)
    {
        light = scene->lights[i];
        l = light->pos - col.coord;

        shadow_col = fire_ray(col.coord, glm::normalize(l), scene);
        color += light->amb * col.obj->amb;

        if (shadow_col == NO_COLLISION || glm::length(l) < glm::length(col.coord - shadow_col.coord))
        {
            phong = calc_phong(light, col.obj, col.coord, view_pos);
            Vec3 r, specular_ref;
            specular_ref = Vec3 { 0.0 };
            if (rec_depth > 0)
            {
                r = Vec3 { glm::reflect(l, glm::normalize(col.obj->get_normal(col.coord))) };                
                Collision spec_col { fire_ray(col.coord, r, scene) };
                if (spec_col == NO_COLLISION)
                {
                    specular_ref = (specular_ref == Vec3{ 0.0 }) ? BACKGROUND_COLOUR : specular_ref;
                }
                else
                {
                    specular_ref = compute_color(spec_col, scene, col.coord, rec_depth - 1);
                }
            }
            
            color += phong + (SPECULARITY * col.obj->spe * specular_ref);
        }
    }

    return Vec3 { fmin(color.x, 1.0), fmin(color.y, 1.0), fmin(color.z, 1.0) };
    //return color;
}


// TODO: Pass in l to avoid another call to normalize + vector subtraction
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

    return (dif + spe);
}