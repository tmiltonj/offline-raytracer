#include <limits>
#include <memory>

#include "raytracer.hpp"



Float2D raytrace(std::shared_ptr<Scene> scene, int &width, int &height)
{
    std::shared_ptr<Camera> cam { scene->camera };
    Vec3 cam_pos { cam->pos };
    int cam_f { cam->f };
    float fov_r { glm::radians((float)(cam->fov)) };

    // Calculate image size
    height = ceil(2.0 * cam->f * tan(fov_r / 2.0));
    width = ceil(cam->a * height);

    // Initialize pixels
    Float2D px_data { new Float1D[width] };
    for (int i = 0; i < width; i++)
        px_data[i] = Float1D(new float[height]);

    // Fire ray for each pixel
    Collision col;
    Vec3 px_screen_space, px_world_space, px_offset, ray_dir;
    px_offset = Vec3 { width / 2, -height / 2, 0 };
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            px_screen_space = Vec3 { x, -y, -cam_f };
            px_world_space = px_screen_space - px_offset;
            ray_dir = glm::normalize(px_world_space - cam_pos);

            col = fire_ray(cam_pos, ray_dir, scene);

            if (col == NO_COLLISION)
            {
                px_data[x][y] = 0.0;
            }
            else
            {
                px_data[x][y] = 1.0;
            }
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
        if (t_candidate >= 0 && t_candidate < t)
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
