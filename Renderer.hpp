//
// Created by Steve Wheeler on 27/08/2023.
//

#pragma once
#include "slib.h"
#include "smath.hpp"
#include "Renderable.hpp"
#include "Mesh.hpp"
#include "constants.h"
#include <utility>
#include <vector>
#include <array>
#include <SDL2/SDL.h>

constexpr unsigned long screenSize = SCREEN_WIDTH*SCREEN_HEIGHT;

class Renderer
{
    void transformRenderable(Renderable& renderable);
    void transformVertex(glm::vec3& v, const glm::vec3& eulerAngles, const glm::vec3& translation, const glm::vec3& scale);
    void transformNormal(glm::vec3& n, const glm::vec3& eulerAngles, const glm::vec3& scale);
    void updateViewMatrix();
    slib::Camera* camera;
    glm::mat4 perspectiveMat;
    glm::mat4 viewMatrix;
    std::vector<Renderable*> renderables;
public:
    bool wireFrame = false;
    explicit Renderer(slib::Camera* _camera) : 
    camera(_camera), perspectiveMat(glm::perspective(fov, aspect, zNear, zFar)),
    viewMatrix(smath::fpsview(camera->pos, camera->rotation.x, camera->rotation.y))
    {}
    void AddRenderable(Renderable& renderable);
    void Render();
    [[nodiscard]] const glm::mat4& GetView() const;
    [[nodiscard]] const glm::mat4& GetPerspective() const;
    
};


