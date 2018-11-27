COMP371 Project - Raytracer
Thabo Milton-Jonas

Offline raytracer built with C++
Implements recursive reflections, antialiasing via supersampling and basic soft shadows.


================
= DEPENDENCIES =
================

- glm
- pthreads (for CImg)


============
= BUILDING =
============
Build with CMake

    mkdir build/
    cd build/
    cmake ..


===========
= RUNNING =
===========
CMake outputs files to bin/

    cd bin/
    ./main INPUT_FILE [OUTPUT_FILE] [REC LEVEL] [SS LEVEL] [SOFT SHADOWS]

    Required arguments
    INPUT_FILE - Location of the scene file to load

    Optional arguments
    OUTPUT_FILE - Filename to save the rendered image to (Default = "Render.bmp")
    REC_LEVEL - Level of recursion (Default = 0)
    SS_LEVEL - Level of supersampling (Default = 1)
    SOFT_SHADOWS - Number of soft shadows (Default = 1)


Any .obj files in the scene files must be located in bin/ otherwise they will not be found


==============
= REFERENCES =
==============
Ray-sphere intersections, Recursive reflections;
Poullis, C. (2018) COMP371 Lecture 13 - Raytracing [PDF]. Department of Computer Science & Software Engineering, Concordia University, QC, Canada.

Ray-plane intersections, Ray-triangle intersection;
Sunday, D. (2012) Intersections of Lines and Planes. Retrieved from: http://www.geomalgorithms.com/a05-_intersect-1.html
Sunday, D. (2012) Intersections of Rays and Triangles (3D). Retrieved from: http://geomalgorithms.com/a06-_intersect-2.html
