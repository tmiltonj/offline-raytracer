#ifndef __SCENELOADER_HPP
#define __SCENELOADER_HPP

#include <deque>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "objects.hpp"

template <typename T>
T pop(std::deque<T> &d) 
{
    T front = d.front();
    d.pop_front();
    return front;
}

template <typename T>
T line_to_single(std::string line, std::string exp_prefix)
{
    std::stringstream ss { line };
    std::string line_prefix;
    T t;

    ss >> line_prefix >> t;

    if (ss.fail())
        throw std::invalid_argument("Could not parse: Line format incorrect");

    if (line_prefix != exp_prefix)
        throw std::invalid_argument("Could not parse: Invalid line prefix");

    return t;
}


std::shared_ptr<Scene> load_scene(std::string filename);
std::vector<std::string> read_file(std::string filename);

Vec3 line_to_vec3(std::string line, std::string exp_prefix);

std::shared_ptr<Camera> parse_camera(std::deque<std::string> &file_deck);
std::shared_ptr<Plane> parse_plane(std::deque<std::string> &file_deck);
std::shared_ptr<Sphere> parse_sphere(std::deque<std::string> &file_deck);
std::shared_ptr<Mesh> parse_mesh(std::deque<std::string> &file_deck);
std::shared_ptr<Mesh> parse_triangle(std::deque<std::string> &file_deck);
std::shared_ptr<Light> parse_light(std::deque<std::string> &file_deck);

#endif