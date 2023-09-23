//
// Created by Steve Wheeler on 30/08/2023.
//

#pragma once

#include <utility>
#include "Mesh.hpp"

struct Renderable
{
    Mesh& mesh;
    glm::vec3 position;
    glm::vec3 eulerAngles;
    glm::vec3 scale;
    slib::Color col;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    glm::vec3 centroid{};
    bool ignoreLighting = false;
    Renderable(Mesh& _mesh, glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale,
               slib::Color _col, std::vector<glm::vec3> _verticies, std::vector<glm::vec3> _normals)
    : mesh(_mesh), position(_position), eulerAngles(_eulerAngles), scale(_scale), col(_col),
      vertices(std::move(_verticies)), normals(std::move(_normals))
    {};

};
