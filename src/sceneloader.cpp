#include <fstream>
#include <sstream>
#include <stdexcept>

#include <iostream>

#include "sceneloader.hpp"

std::shared_ptr<Scene> load_scene(std::string filename)
{
    // Read file & make sure we have data to parse
    std::vector<std::string> file_list { read_file(filename) };
    if (file_list.size() == 0)
        throw std::invalid_argument("File was empty");
    
    // Convert to a deque for easier sequential reading    
    std::deque<std::string> file_deck { file_list.begin(), file_list.end() };

    // Determine how many objects in the file
    int num_objects;
    std::stringstream line { pop(file_deck) };
    line >> num_objects;
    if (line.fail())
        throw std::invalid_argument("Could not read number of objects");

    /* MAIN PARSING LOOP
     * Each block starts with the object type on it's own line.
     * The parse_xxx functions will pop the correct number of lines
     * from file_deck to get to the next entity name since we
     * stop and return nullptr if there are any issues parsing
     */ 
    std::shared_ptr<Scene> scene { std::make_shared<Scene>() };

    try
    {
        std::string ent_type;
        for (int i = 0; i < num_objects; i++)
        {
            ent_type = pop(file_deck);

            if (ent_type == "camera") {
                scene->camera = parse_camera(file_deck);
            }
            else if (ent_type == "plane") {
                scene->objects.push_back(parse_plane(file_deck));
            }
            else if (ent_type == "sphere") {
                scene->objects.push_back(parse_sphere(file_deck));
            }
            else if (ent_type == "light") {
                scene->lights.push_back(parse_light(file_deck));
            }
            else if (ent_type == "mesh") {
                scene->objects.push_back(parse_mesh(file_deck));
            }
            else if (ent_type == "triangle") {
                scene->objects.push_back(parse_triangle(file_deck));
            }
            else {
                throw std::invalid_argument("Unknown entity type '" + ent_type + "'");
            }
        }
    }
    catch (const std::invalid_argument& e) { throw e; }
    
    return scene;
}



std::vector<std::string> read_file(std::string filename)
{
    std::vector<std::string> file_list;
    std::ifstream file;

    file.open(filename);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (line.back() == '\r') line.pop_back();
            file_list.push_back(line);
        }

        file.close();
    }

    return file_list;
}


Vec3 line_to_vec3(std::string line, std::string exp_prefix)
{
    std::stringstream ss { line };
    std::string line_prefix;
    Vec3 v;

    ss >> line_prefix >> v.x >> v.y >> v.z;

    if (ss.fail())
        throw std::invalid_argument("Could not parse; Line format incorrect");

    if (line_prefix != exp_prefix)
        throw std::invalid_argument("Could not parse; Invalid line prefix");

    return v;
}


/* Pops the first 4 lines from file_deck and tries to parse them into
 * a Camera object.
 * 
 * Line format is as follows (or an std::invalid_argument is thrown)
 *  pos: px py pz
 *  fov: theta
 *  f: focal_length
 *  a: aspect_ratio
 */
std::shared_ptr<Camera> parse_camera(std::deque<std::string> &file_deck)
{
    try
    {
        Vec3 pos { line_to_vec3(pop(file_deck), "pos:") };
        int fov { line_to_single<int>(pop(file_deck), "fov:") };
        int f { line_to_single<int>(pop(file_deck), "f:") };
        float a { line_to_single<float>(pop(file_deck), "a:") };

        return std::make_shared<Camera>(pos, fov, f, a);
    }
    catch (const std::invalid_argument &e){ throw e; }
}



std::shared_ptr<Plane> parse_plane(std::deque<std::string> &file_deck)
{
    try
    {
        Vec3 normal { line_to_vec3(pop(file_deck), "nor:") };
        Vec3 point { line_to_vec3(pop(file_deck), "pos:") };
        Vec3 amb { line_to_vec3(pop(file_deck), "amb:") };
        Vec3 dif { line_to_vec3(pop(file_deck), "dif:") };
        Vec3 spe { line_to_vec3(pop(file_deck), "spe:") };
        float shi { line_to_single<float>(pop(file_deck), "shi:") };

        return std::make_shared<Plane>(normal, point, amb, dif, spe, shi);
    }
    catch (const std::invalid_argument &e){ throw e; }   
}



std::shared_ptr<Sphere> parse_sphere(std::deque<std::string> &file_deck)
{
    try
    {
        Vec3 pos { line_to_vec3(pop(file_deck), "pos:") };
        float r { line_to_single<float>(pop(file_deck), "rad:") };
        Vec3 amb { line_to_vec3(pop(file_deck), "amb:") };
        Vec3 dif { line_to_vec3(pop(file_deck), "dif:") };
        Vec3 spe { line_to_vec3(pop(file_deck), "spe:") };
        float shi { line_to_single<float>(pop(file_deck), "shi:") };

        return std::make_shared<Sphere>(pos, r, amb, dif, spe, shi);
    }
    catch (const std::invalid_argument &e){ throw e; }   
}



std::shared_ptr<Mesh> parse_mesh(std::deque<std::string> &file_deck)
{
    try
    {
        std::string filename { pop(file_deck) };

        Vec3 amb, dif, spe;
        amb = Vec3 { line_to_vec3(pop(file_deck), "amb:") };
        dif = Vec3 { line_to_vec3(pop(file_deck), "dif:") };
        spe = Vec3 { line_to_vec3(pop(file_deck), "spe:") };
        float shi { line_to_single<float>(pop(file_deck), "shi:") };

        return std::make_shared<Mesh>(filename, amb, dif, spe, shi);
    }
    catch (const std::invalid_argument &e){ throw e; }
}



std::shared_ptr<Mesh> parse_triangle(std::deque<std::string> &file_deck)
{
    try
    {
        std::vector<Vec3> vertices;
        vertices.push_back(line_to_vec3(pop(file_deck), "v1:"));
        vertices.push_back(line_to_vec3(pop(file_deck), "v2:"));
        vertices.push_back(line_to_vec3(pop(file_deck), "v3:"));

        Vec3 amb, dif, spe;
        amb = Vec3 { line_to_vec3(pop(file_deck), "amb:") };
        dif = Vec3 { line_to_vec3(pop(file_deck), "dif:") };
        spe = Vec3 { line_to_vec3(pop(file_deck), "spe:") };
        float shi { line_to_single<float>(pop(file_deck), "shi:") };

        return std::make_shared<Mesh>(vertices, amb, dif, spe, shi);
    }
    catch (const std::invalid_argument &e){ throw e; }
}



std::shared_ptr<Light> parse_light(std::deque<std::string> &file_deck)
{
    try
    {
        Vec3 pos { line_to_vec3(pop(file_deck), "pos:") };
        Vec3 amb { line_to_vec3(pop(file_deck), "amb:") };
        Vec3 dif { line_to_vec3(pop(file_deck), "dif:") };
        Vec3 spe { line_to_vec3(pop(file_deck), "spe:") };

        return std::make_shared<Light>(pos, amb, dif, spe);
    }
    catch (const std::invalid_argument &e){ throw e; }   
}