//
// Created by Steve Wheeler on 23/08/2023.
//
#pragma once
#include <utility>
#include <vector>
#include "slib.h"
#include "smath.hpp"

struct Mesh
{
    const std::vector<glm::vec3> vertices;
    const std::vector<slib::tri> faces; // Contains the indices of the vertices and texture data
    const std::vector<glm::vec2> textureCoords;
    const std::vector<glm::vec3> normals; // The normal shares the same index as the associated vertex in 'vertices'
    const slib::texture texture;
    const glm::vec3 centroid;
    Mesh(const std::vector<glm::vec3>& _vertices, const std::vector<slib::tri>& _faces,
         const std::vector<glm::vec2>& _textureCoords, const std::vector<glm::vec3>& _normals, slib::texture  _texture) :
        vertices(_vertices), faces(_faces), textureCoords(_textureCoords), normals(_normals),
        texture(std::move(_texture)), centroid(smath::getCentroid(vertices))
    {
    }
};
