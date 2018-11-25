#include <assert.h>
#include <iostream>

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>

#include "sceneloader.hpp"
#include "objects.hpp"
#include "raytracer.hpp"

const float EPSILON { 0.01 };

void test_raytrace();
void test_fire_ray();

int main()
{
    std::cout << "Testing fire_ray()... ";
    test_fire_ray();
    std::cout << "PASS" << std::endl;

    std::cout << "Testing raytrace()... ";
    test_raytrace();
    std::cout << "PASS" << std::endl;

    return 0;
}


void test_raytrace()
{
    std::shared_ptr<Scene> sc { load_scene("../../test/scenes/test_raytrace.txt") };
    int width, height, exp_width, exp_height;
    exp_width = 1537;
    exp_height = 1155;
    
    Float2D px_data { raytrace(sc, width, height) };

    assert (exp_height == height);
    assert (exp_width == width);

    // Only for B&W rendering
    /*
    assert (abs(px_data[exp_width / 2][exp_height / 2]) < EPSILON);
    assert (abs(px_data[exp_width / 2][10] - 1.0) < EPSILON);
    */
}


void test_fire_ray()
{
    std::shared_ptr<Scene> sc { load_scene("../../test/scenes/test_fire_ray.txt") };
    Vec3 p0 { 0.0 };
    Vec3 d, exp_pos;
    Collision exp_col, c;
    std::shared_ptr<Object> exp_obj;

    // Case 1: Ray hits sphere (with plane behind)
    d = glm::normalize(Vec3 { -1.0, 3.0, -4.0 });
    exp_obj = sc->objects[0];
    exp_pos = Vec3 { -1.0, 3.0, -4.0 };

    c = fire_ray(p0, d, sc);

    assert (c.obj == exp_obj);
    assert (glm::length(c.coord - exp_pos) < EPSILON);


    // Case 2: Ray hits plane
    d = Vec3 { 1.0, 0.0, -1.0 };
    exp_obj = sc->objects[1];
    exp_pos = Vec3 { 10.0, 0.0, -10.0 };

    c = fire_ray(p0, d, sc);

    assert (c.obj == exp_obj);
    assert (glm::length(c.coord - exp_pos) < EPSILON);


    // Case 3: Ray does not hit anything
    d = Vec3 { 0.0, 1.0, 0.0 };

    c = fire_ray(p0, d, sc);

    assert (c == NO_COLLISION);

}