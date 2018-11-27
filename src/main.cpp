#include <iostream>
#include <memory>

#include "CImg.h"

#include "objects.hpp"
#include "raytracer.hpp"
#include "sceneloader.hpp"


const int NUM_CHANNELS { 3 };
const int MIN_ARGS { 1 };
const int DEFAULT_RECURSION_LEVEL { 0 };
const int DEFAULT_SSAMPLE_LEVEL { 1 };


int main(int argc, char *argv[])
{
    if (argc < MIN_ARGS + 1)
        return 1;

    std::string scene_file { argv[1] };


    int recursion_level { DEFAULT_RECURSION_LEVEL };
    if (argc > 2)
    {
        try {
            recursion_level = std::stoi(argv[2]);
            std::cout << "Setting recursion level to " << recursion_level << std::endl;
        }
        catch (const std::invalid_argument &e){}
        catch (const std::out_of_range &e){}
    }


    int ssample_level { DEFAULT_SSAMPLE_LEVEL };
    if (argc > 3)
    {
        try {
            ssample_level = std::stoi(argv[3]);
            std::cout << "Setting supersampling level to " << ssample_level << "x" << std::endl;
        }
        catch (const std::invalid_argument &e){}
        catch (const std::out_of_range &e){}
    }
   
    
    try 
    {
        std::shared_ptr<Scene> sc { load_scene(scene_file) };

        int width, height;
        Pixel2D px_data { raytrace(sc, width, height, recursion_level, ssample_level) };

        cimg_library::CImg<float> image(width, height, 1, NUM_CHANNELS, 0);

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                for (int z = 0; z < NUM_CHANNELS; z++) {
                    image(x, y, z) = px_data[x][y][z] * 255.0f;
                }
            }
        }

        image.save("render.bmp");

        cimg_library::CImgDisplay main_disp { image, "Render" };
        while (!main_disp.is_closed()){ main_disp.wait(); }

    }
    catch (const std::invalid_argument &e)
    {
        return 2;
    }

    return 0;
}