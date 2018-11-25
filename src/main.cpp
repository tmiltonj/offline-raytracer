#include <iostream>
#include <memory>

#include "CImg.h"

#include "objects.hpp"
#include "raytracer.hpp"
#include "sceneloader.hpp"


const int NUM_CHANNELS { 3 };


int main()
{
    std::shared_ptr<Scene> sc { 
        load_scene("../scenes/scene2.txt") 
        };


    int width, height;
    Float2D px_data { 
        raytrace(sc, width, height) 
        };


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

    return 0;
}