#include <assert.h>
#include <iostream>

#include "objects.hpp"

void test_plane();
void test_sphere();

int main()
{
    std::cout << "Testing Plane... ";
    test_plane();
    std::cout << "PASS" << std::endl;

    std::cout << "Testing Sphere... ";
    test_sphere();
    std::cout << "PASS" << std::endl;

    return 0;
}

void test_plane()
{
    Plane p { 
        Vector { 1.0, 2.0, 3.0 }, 
        Coord { 11.0, 12.0, 13.0 },
        glm::vec3 { 21.0, 22.0, 23.0 }, 
        glm::vec3 { 31.0, 32.0, 33.0 },
        glm::vec3 { 41.0, 42.0, 43.0 },
        5.0 };

    // Test get_normal
    Vector exp_normal = Vector { 1.0, 2.0, 3.0 };
    assert (exp_normal == p.get_normal(Coord { 0.0 }));

    // Test check_collision
    float exp_collision = 1.0;
    assert (exp_collision == p.check_collision(Coord { 0.0 }, Vector { 0.0 }));

    // Test constructors
    // Invalid amb
    bool inst_failed = false;
    try {
        Plane p2 {
            Vector { 1.0 },
            Coord { 11.0 },
            glm::vec3 { -1.0, 1.0, 1.0 },
            glm::vec3 { 1.0 },
            glm::vec3 { 1.0 },
            5.0 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);

    // Invalid dif
    inst_failed = false;
    try {
        Plane p2 {
            Vector { 1.0 },
            Coord { 11.0 },
            glm::vec3 { 1.0 },
            glm::vec3 { 1.0, -1.0, 1.0 },
            glm::vec3 { 1.0 },
            5.0 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);
    
    // Invalid spe
    inst_failed = false;
    try {
        Plane p2 {
            Vector { 1.0 },
            Coord { 11.0 },
            glm::vec3 { 1.0 },
            glm::vec3 { 1.0 },
            glm::vec3 { 1.0, 1.0, -1.0 },
            5.0 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);
    
    // Invalid shi
    inst_failed = false;
    try {
        Plane p2 {
            Vector { 1.0 },
            Coord { 11.0 },
            glm::vec3 { 1.0 }, glm::vec3 { 1.0 }, glm::vec3 { 1.0 },
            0.0 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);
    
}

void test_sphere()
{
    Sphere s {
        Coord { 1.0 },
        1.0,
        glm::vec3 { 1.0 }, glm::vec3 { 1.0 }, glm::vec3 { 1.0 },
        5.0 };

    // Test get_normal
    Vector exp_normal = Vector { 1.0 };
    assert (exp_normal == s.get_normal(Coord { 1.0 }));

    // Test check_collision
    float exp_collision = 1.0;
    assert (exp_collision == s.check_collision(Coord { 1.0 }, Vector { 1.0 }));

    // Test instantiation
    // Invalid r
    bool inst_failed = false;

    try {
        Sphere s {
            Coord { 1.0 },
            0.0,
            glm::vec3 { 1.0 }, glm::vec3 { 1.0 }, glm::vec3 { 1.0 },
            5.0 };
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }

    assert (inst_failed);
}