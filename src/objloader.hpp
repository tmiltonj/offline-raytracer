#ifndef SHADERTEST_OBJLOADER_H
#define SHADERTEST_OBJLOADER_H

#include <vector>
#include <glm/glm.hpp>

bool loadOBJ(std::string path,
        std::vector<glm::vec3> &out_vertices,
        std::vector<glm::vec3> &out_normals,
        std::vector<glm::vec2> &out_uvs,
        std::vector<int> &out_indices);

#endif
