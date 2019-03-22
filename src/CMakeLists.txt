set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(
    main
    main.cpp
    objects.cpp
    raytracer.cpp
    sceneloader.cpp
    objloader.cpp
    )

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})
target_link_libraries(main ${X11_LIBRARIES})

find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIR})
target_link_libraries(main ${GLM_LIBRARIES})

find_package(Threads REQUIRED)
target_link_libraries(main ${CMAKE_THREAD_LIBS_INIT})