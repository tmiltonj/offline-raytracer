set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(
    main
    main.cpp
    objects.cpp
    raytracer.cpp
    sceneloader.cpp
    )
    
find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})
target_link_libraries(main ${X11_LIBRARIES})

find_package(Threads REQUIRED)
target_link_libraries(main ${CMAKE_THREAD_LIBS_INIT})