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
    float exp_collision = 1.0;
    assert (exp_collision == p.check_collision(Vec3 { 0.0 }, Vec3 { 0.0 }));

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
    float exp_collision = 1.0;
    assert (exp_collision == s.check_collision(Vec3 { 1.0 }, Vec3 { 1.0 }));

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