# Offline Raytracer
Offline Raytracer for Linux built in C++. Modified from my final project for my Computer Graphics unit.

Implements recursive reflections, antialiasing via supersampling and basic soft shadows.

## Prerequisites

* glm (OpenGL Mathematics Library)

Installation instructions can be found at https://glm.g-truc.net/

## Getting started

Build the project using CMake;

    mkdir build/
    cd build/
    cmake ..
    make


## Basic usage

Executable is built to `bin/`. Any .obj files in the scene files must be located in `bin/` to be loaded

    ./main <input_file> [output_file] [recursion] [ss level] [soft shadows]

Run a basic raytrace

    ./main ../scenes/scene1.txt

Save the results to an image

    ./main ../scenes/scene2.txt scene2.bmp

Run a raytrace with 4 levels of recursion, 2x supersampling and 5 passes for soft shadows

    ./main ../scenes/scene5.txt scene5.bmp 4 2 5


## Scene files

Scene files are plain-text files formatted according to the following assignment;

The scene file describes the contents of the scene. The scene can include the following objects: 
* One camera
* One plane
* One mesh
* Any number of lights
* Any number of spheres

The first line in the file is a number indicating the total number of objects in the scene. Related information about each object is then specified from line 2 on-wards as follows:

### Camera
```
camera
pos: x y z //where x y z are floating point numbers specifying the location of the camera
fov: theta //where theta is the field-of-view in degrees)
f: focal_length //where d is the focal length of the camera
a: aspect_ratio //where a is the aspect ratio of the camera
```
### Plane
```
plane
nor: nx ny nz //where (nx, ny, nz) is the normal
pos: px py pz //where (px, py, pz) is the position of a point on the plane
amb: ax ay az //where (ax, ay, az) is the ambient color of the plane
dif: dx dy dz //where (dx, dy, dx) is the diffuse color of the plane
spe: sx sy sz //where (sx, sy, sz) is the specular color of the plane
shi: s //where s is the specular shininess factor
```

### Sphere
```
sphere
pos: px py pz //where (px, py, pz) is the position of the center of the sphere
rad: r //where r is the radius of the sphere
amb: ax ay az //where (ax, ay, az) is the ambient color of the sphere
dif: dx dy dz //where (dx, dy, dx) is the diffuse color of the sphere
spe: sx sy sz //where (sx, sy, sz) is the specular color of the sphere
shi: s //where s is the specular shininess factor
```

### Mesh
```
mesh
file: filename.obj //where filename.obj is the OBJ file containing the mesh
amb: ax ay az //where (ax, ay, az) is the ambient color of the mesh
dif: dx dy dz //where (dx, dy, dx) is the diffuse color of the mesh
spe: sx sy sz //where (sx, sy, sz) is the specular color of the mesh
shi: s //where s is the specular shininess factor
```

### Light
```
light
pos: px py pz //where (px, py, pz) is the position of the light
col: cx cy cz //where (cx, cy, cz) is the color of the light
```

## References
Ray-sphere intersections, Recursive reflections;
* Poullis, C. (2018) COMP371 Lecture 13 - Raytracing [PDF]. Department of Computer Science & Software Engineering, Concordia University, QC, Canada.

Ray-plane intersections, Ray-triangle intersection;
* Sunday, D. (2012) Intersections of Lines and Planes. Retrieved from: http://www.geomalgorithms.com/a05-_intersect-1.html
* Sunday, D. (2012) Intersections of Rays and Triangles (3D). Retrieved from: http://geomalgorithms.com/a06-_intersect-2.html
