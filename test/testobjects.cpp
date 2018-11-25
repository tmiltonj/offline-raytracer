#include <assert.h>
#include <iostream>

#include "objects.hpp"

void test_camera();
void test_light();
void test_scene();
void test_plane();
void test_sphere();

int main()
{
    std::cout << "Testing Camera... ";
    test_camera();
    std::cout << "PASS" << std::endl;
    
    std::cout << "Testing Light... ";
    test_light();
    std::cout << "PASS" << std::endl;

    std::cout << "Testing Scene... ";
    test_scene();
    std::cout << "PASS" << std::endl;

    std::cout << "Testing Plane... ";
    test_plane();
    std::cout << "PASS" << std::endl;

    std::cout << "Testing Sphere... ";
    test_sphere();
    std::cout << "PASS" << std::endl;

    return 0;
}


void test_camera()
{
    // Test constructors
    // Invalid fov (= 0)
    bool inst_failed = false;
    try {
        Camera c {
            Vec3 { 1.0 },
            0,
            1,
            1.33 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);
    
    // Invalid f (= 0)
    try {
        Camera c {
            Vec3 { 1.0 },
            60,
            0,
            1.33 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);
    
    // Invalid a (= 0.0)
    try {
        Camera c {
            Vec3 { 1.0 },
            60,
            1,
            0.0 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);
}


void test_light()
{
    // Test valid_light
    Vec3 v_light {  0.1 };
    Vec3 i_light { -0.1 };

    assert (valid_light(v_light));
    assert (!valid_light(i_light));

    // Test constructors
    // Valid params
    bool inst_failed = false;
    try {
        Light l {
            Vec3 { 1.0 },
            v_light, v_light, v_light
        };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }
    assert (!inst_failed);

    // Invalid amb
    inst_failed = false;
    try {
        Light l {
            Vec3 { 1.0 },
            i_light, v_light, v_light
        };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }
    assert (inst_failed);
    
    // Invalid dif
    inst_failed = false;
    try {
        Light l {
            Vec3 { 1.0 },
            v_light, i_light, v_light
        };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }
    assert (inst_failed);
    
    // Invalid spe
    inst_failed = false;
    try {
        Light l {
            Vec3 { 1.0 },
            v_light, v_light, i_light
        };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }
    assert (inst_failed);
}

void test_scene()
{
    // Test destructor and shared_ptrs are working properly
    Scene s;
    s.camera = std::make_shared<Camera>(
        Vec3 { 0.0 },
        60,
        1000,
        1.33 );

    s.objects.push_back(
        std::make_shared<Plane>(
            Vec3 { 1.0 }, 
            Vec3 { 1.0 },
            Vec3 { 1.0 }, 
            Vec3 { 1.0 }, 
            Vec3 { 1.0 },
            5.0)
    );
    
    s.objects.push_back(
        std::make_shared<Plane>(
            Vec3 { 2.0 }, 
            Vec3 { 2.0 },
            Vec3 { 2.0 }, 
            Vec3 { 2.0 }, 
            Vec3 { 2.0 },
            25.0)
    );

}


void test_plane()
{
    Plane p { 
        Vec3 { 1.0, 2.0, 3.0 }, 
        Vec3 { 11.0, 12.0, 13.0 },
        Vec3 { 21.0, 22.0, 23.0 }, 
        Vec3 { 31.0, 32.0, 33.0 },
        Vec3 { 41.0, 42.0, 43.0 },
        5.0 };

    // Test get_normal
    Vec3 exp_normal = Vec3 { 1.0, 2.0, 3.0 };
    assert (exp_normal == p.get_normal(Vec3 { 0.0 }));

    // Test check_collision
    Plane p2 {
        Vec3 { 0, 1, 0 }, // normal
        Vec3 { 1, 1, 1 }, // point on plane
        Vec3 { 0.0 }, Vec3 { 0.0 }, Vec3 { 0.0 }, 1.0
    };

    // Case 1: 45-degree ray collides with plane
    Vec3 p0 { 1, 2, 1 };
    Vec3 d0 { 1, -1, 0 };
    float col_result = p2.check_collision(p0, d0);
    assert ( (p0 + d0 * col_result) == (Vec3 { 2, 1, 1 }) );

    // Case 2: Ray fired in opposite direction, no collision
    Vec3 d1 { 1, 2, 0 };
    col_result = p2.check_collision(p0, d1);
    assert ( (col_result - (-0.5)) < 0.01 ); // col_result < 0.0

    // Case 3: Ray is perpendicular to plane, no collision
    Vec3 d2 { 1, 0, -1 };
    col_result = p2.check_collision(p0, d2);
    assert (col_result < 0.0);
    
    // Test constructors
    // Invalid amb
    bool inst_failed = false;
    try {
        Plane p2 {
            Vec3 { 1.0 },
            Vec3 { 11.0 },
            Vec3 { -1.0, 1.0, 1.0 },
            Vec3 { 1.0 },
            Vec3 { 1.0 },
            5.0 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);

    // Invalid dif
    inst_failed = false;
    try {
        Plane p2 {
            Vec3 { 1.0 },
            Vec3 { 11.0 },
            Vec3 { 1.0 },
            Vec3 { 1.0, -1.0, 1.0 },
            Vec3 { 1.0 },
            5.0 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);
    
    // Invalid spe
    inst_failed = false;
    try {
        Plane p2 {
            Vec3 { 1.0 },
            Vec3 { 11.0 },
            Vec3 { 1.0 },
            Vec3 { 1.0 },
            Vec3 { 1.0, 1.0, -1.0 },
            5.0 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);
    
    // Invalid shi
    inst_failed = false;
    try {
        Plane p2 {
            Vec3 { 1.0 },
            Vec3 { 11.0 },
            Vec3 { 1.0 }, Vec3 { 1.0 }, Vec3 { 1.0 },
            0.0 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);
    
}


void test_sphere()
{
    Sphere s {
        Vec3 { 1.0 },
        1.0,
        Vec3 { 1.0 }, Vec3 { 1.0 }, Vec3 { 1.0 },
        5.0 };

    // Test get_normal
    Vec3 exp_normal = Vec3 { 1.0 };
    assert (exp_normal == s.get_normal(Vec3 { 1.0 }));

    // Test check_collision
    // Case 1: Collision on top point of the sphere, in front
    Sphere s2 {
        Vec3 { 3.0, 1.0, 3.0 }, // pos
        1.0, // r
        Vec3 { 1.0 }, Vec3 { 1.0 }, Vec3 { 1.0 }, 5.0 };

    Vec3 p0 { 1, 2, 3 };
    Vec3 d  { 1, 0, 0 };

    float col_result = s2.check_collision(p0, d);
    assert ( (p0 + d * col_result == Vec3 { 3, 2, 3}) );

    // Case 2: Collision in middle of sphere, in front
    Sphere s3 {
        Vec3 { 4.0, 2.0, 3.0 }, // pos
        2.0, // r
        Vec3 { 1.0 }, Vec3 { 1.0 }, Vec3 { 1.0 }, 5.0 };

    col_result = s3.check_collision(p0, d);
    assert ( (p0 + d * col_result == Vec3 { 2, 2, 3}) );

    // Case 3: Sphere is behind the ray, no collision
    Sphere s4 {
        Vec3 { -4.0, 2.0, 3.0 }, // pos
        2.0, // r
        Vec3 { 1.0 }, Vec3 { 1.0 }, Vec3 { 1.0 }, 5.0 };

    col_result = s4.check_collision(p0, d);
    assert (col_result < 0.0);

    // Case 4: Sphere is in front of the ray, but not in the way of the ray
    Sphere s5 {
        Vec3 { 4.0, 5.0, 5.0 }, // pos
        2.0, // r
        Vec3 { 1.0 }, Vec3 { 1.0 }, Vec3 { 1.0 }, 5.0 };

    col_result = s5.check_collision(p0, d);
    assert (col_result < 0.0);


    // Test instantiation
    // Invalid r
    bool inst_failed = false;

    try {
        Sphere s {
            Vec3 { 1.0 },
            0.0,
            Vec3 { 1.0 }, Vec3 { 1.0 }, Vec3 { 1.0 },
            5.0 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);
}