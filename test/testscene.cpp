#include <assert.h>
#include <iostream>

#include "scene.hpp"

void test_camera();
void test_scene();

int main()
{
    std::cout << "Testing camera... ";
    test_camera();
    std::cout << "PASS" << std::endl;

    std::cout << "Testing scene... ";
    test_scene();
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
            Coord { 1.0 },
            0,
            1,
            1.33 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);
    
    // Invalid f (= 0)
    try {
        Camera c {
            Coord { 1.0 },
            60,
            0,
            1.33 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);
    
    // Invalid a (= 0.0)
    try {
        Camera c {
            Coord { 1.0 },
            60,
            1,
            0.0 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);
}

void test_scene()
{
    // Test destructor and shared_ptrs are working properly
    Scene s;
    s.camera = std::make_shared<Camera>(
        Coord { 0.0 },
        60,
        1000,
        1.33 );

    s.objects.push_back(
        std::make_shared<Plane>(
            Vector { 1.0 }, 
            Coord { 1.0 },
            glm::vec3 { 1.0 }, 
            glm::vec3 { 1.0 }, 
            glm::vec3 { 1.0 },
            5.0)
    );
    
    s.objects.push_back(
        std::make_shared<Plane>(
            Vector { 2.0 }, 
            Coord { 2.0 },
            glm::vec3 { 2.0 }, 
            glm::vec3 { 2.0 }, 
            glm::vec3 { 2.0 },
            25.0)
    );

}