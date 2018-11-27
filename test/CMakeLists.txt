include_directories(${PROJECT_SOURCE_DIR}/src)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -O2 -std=c++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/test/")

add_executable(
    testobjects
    testobjects.cpp
    ../src/objects.cpp
    ../src/objloader.cpp
)

add_executable(
    testloader
    testloader.cpp
    ../src/sceneloader.cpp
    ../src/objects.cpp
    ../src/objloader.cpp
)

add_executable(
    testray
    testray.cpp
    ../src/sceneloader.cpp
    ../src/objects.cpp
    ../src/raytracer.cpp
    ../src/objloader.cpp
)