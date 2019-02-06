#include <iostream>
#include <memory>

#include "CImg.h"

#include "objects.hpp"
#include "raytracer.hpp"
#include "sceneloader.hpp"


// Don't change this
const int NUM_CHANNELS { 3 };
const int NUM_ARGS { 1 };


<<<<<<< HEAD
// Defaults for cmd-line arguments
const int MIN_ARGS { 1 };

const std::string DEFAULT_OUTPUT_FILENAME { "Render.bmp" };
const int DEFAULT_RECURSION_LEVEL { 0 };
const int DEFAULT_SSAMPLE_LEVEL { 1 };
const int DEFAULT_SOFT_SHADOWS { 1 };


int main(int argc, char *argv[])
{
    // Parse command-line arguments
    if (argc < MIN_ARGS + 1)
    {
        std::cerr << "Missing scene filename" << std::endl;
        return 1;
    }
    std::string scene_file { argv[1] };
=======
int main(int argc, char *argv[])
{
    if (argc < NUM_ARGS + 1)
        return 1;

    std::string scene_file { argv[1] };
    
    std::shared_ptr<Scene> sc { 
        load_scene(scene_file)
        };
>>>>>>> fba0a9db07a085e32e905f3338a13ffc3d8eb128


    std::string output_filename;
    output_filename = (argc > 2) ? argv[2] : DEFAULT_OUTPUT_FILENAME;


    int recursion_level { DEFAULT_RECURSION_LEVEL };
    if (argc > 3)
    {
        try {
            recursion_level = std::stoi(argv[3]);
            std::cout << "Setting recursion level to " << recursion_level << std::endl;
        }
        catch (const std::invalid_argument &e){ std::cerr << "Invalid recursion level, using default (" << DEFAULT_RECURSION_LEVEL << ")\n"; }
        catch (const std::out_of_range &e){ std::cerr << "Invalid recursion level, using default (" << DEFAULT_RECURSION_LEVEL << ")\n"; }
    }


    int ssample_level { DEFAULT_SSAMPLE_LEVEL };
    if (argc > 4)
    {
        try {
            ssample_level = std::stoi(argv[4]);
            std::cout << "Setting supersampling level to " << ssample_level << "x" << std::endl;
        }
        catch (const std::invalid_argument &e){ std::cerr << "Invalid supersample level, using default (" << DEFAULT_SSAMPLE_LEVEL << ")\n"; }
        catch (const std::out_of_range &e){ std::cerr << "Invalid supersample level, using default (" << DEFAULT_SSAMPLE_LEVEL << ")\n"; }
    }


    int sshadow_level { DEFAULT_SOFT_SHADOWS };
    if (argc > 5)
    {
        try {
            sshadow_level = std::stoi(argv[5]);
            std::cout << "Setting number of soft shadows to " << sshadow_level << std::endl;
        }
        catch (const std::invalid_argument &e){std::cerr << "Invalid soft shadow level, using default (" << DEFAULT_SOFT_SHADOWS << ")\n";}
        catch (const std::out_of_range &e){std::cerr << "Invalid soft shadow level, using default (" << DEFAULT_SOFT_SHADOWS << ")\n";}
    }
   


    // RAYTRACING 
    try 
    {
        std::shared_ptr<Scene> sc { load_scene(scene_file) };

        int width, height;
        Pixel2D px_data { 
            raytrace(sc, width, height, recursion_level, ssample_level, sshadow_level) 
        };

        cimg_library::CImg<float> image(width, height, 1, NUM_CHANNELS, 0);

        // Scale 0-1 colour values to 0-255 for bitmap output
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                for (int z = 0; z < NUM_CHANNELS; z++) {
                    image(x, y, z) = px_data[x][y][z] * 255.0f;
                }
            }
        }

        image.save(output_filename.c_str());

        cimg_library::CImgDisplay main_disp { image, "Render" };
        while (!main_disp.is_closed()){ main_disp.wait(); }

    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Could not raytrace " << scene_file << ": " << e.what() << "\n";
        return 2;
    }

    return 0;
}