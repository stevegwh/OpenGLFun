//
// Created by Steve Wheeler on 27/08/2023.
//

#include "Renderer.hpp"
#include <cmath>
#include <algorithm>
#include "constants.h"

// Essentially the same as transformNormal but no translation, doesnt multiply by viewMatrix and homog coord is 0.
void Renderer::transformNormal(glm::vec3& n, const glm::vec3& eulerAngles, const glm::vec3& scale)
{
    const float xrad = eulerAngles.x * RAD;
    const float yrad = eulerAngles.y * RAD;
    const float zrad = eulerAngles.z * RAD;
    const float axc = std::cos(xrad);
    const float axs = std::sin(xrad);
    const float ayc = std::cos(yrad);
    const float ays = -std::sin(yrad);
    const float azc = std::cos(zrad);
    const float azs = -std::sin(zrad);

    // Only rotation and scaling, no translation for normals
    glm::mat4  transformMatrix({
                                   { scale.x * (ayc * azc), scale.y * (ayc * azs), -scale.z * ays, 0 },
                                   { scale.x * (axs * ays * azc - axc * azs), scale.y * (axs * ays * azs + axc * azc), scale.z * axs * ayc, 0 },
                                   { scale.x * (axc * ays * azc + axs * azs), scale.y * (axc * ays * azs - axs * azc), scale.z * axc * ayc, 0 },
                                   { 0, 0, 0, 1.0f } // Homogeneous coordinate
                               });

    // If you have non-uniform scaling, get the inverse transpose matrix
    //slib::mat normalTransformMatrix = inverseTranspose(transformMatrix);

    // Convert the normal to a homogeneous coordinate (vec4)
    glm::vec4 n4({n.x, n.y, n.z, 0 }); // w is 0 for direction vectors

    // Apply transformation
    //auto transformedNormal = normalTransformMatrix * n4;
    auto transformedNormal = transformMatrix * n4;

    n = { transformedNormal.x, transformedNormal.y, transformedNormal.z };
}


void Renderer::transformVertex(glm::vec3& v, const glm::vec3& eulerAngles, const glm::vec3& translation,
                               const glm::vec3& scale)
{
    const float xrad = eulerAngles.x * RAD;
    const float yrad = eulerAngles.y * RAD;
    const float zrad = eulerAngles.z * RAD;
    const float axc = std::cos(xrad);
    const float axs = std::sin(xrad);
    const float ayc = std::cos(yrad);
    const float ays = -std::sin(yrad);
    const float azc = std::cos(zrad);
    const float azs = -std::sin(zrad);
    const float transx = translation.x;
    const float transy = translation.y;
    const float transz = translation.z;

    glm::mat4  transformMatrix({
                                { scale.x * (ayc * azc), scale.y * (ayc * azs), -scale.z * ays, transx },
                                { scale.x * (axs * ays * azc - axc * azs), scale.y * (axs * ays * azs + axc * azc), scale.z * axs * ayc, transy },
                                { scale.x * (axc * ays * azc + axs * azs), scale.y * (axc * ays * azs - axs * azc), scale.z * axc * ayc, transz },
                                { 0, 0, 0, 1.0f } // Homogeneous coordinate
                                });

    glm::vec4 v4({v.x, v.y, v.z, 1 });

    auto transformedVector =  viewMatrix * transformMatrix * v4;
    
    v = { transformedVector.x, transformedVector.y, transformedVector.z };
}

inline void Renderer::transformRenderable(Renderable& renderable)
{
    bool hasNormalData = !renderable.mesh.normals.empty();
    for (int i = 0; i < renderable.vertices.size(); i++)
    {
        transformVertex(renderable.vertices[i], renderable.eulerAngles, renderable.position, renderable.scale);
        if (!hasNormalData) continue;
        transformNormal(renderable.normals[i], renderable.eulerAngles, renderable.scale);
    }
}

inline void createProjectedSpace(const Renderable& renderable, const glm::mat4& perspectiveMat, std::vector<glm::vec4>& projectedPoints)
{
    // Make projected space
    for (const auto &v : renderable.vertices) 
    {
        projectedPoints.push_back(perspectiveMat * (glm::vec4){v.x, v.y, v.z, 1});
    }
}

inline void Renderer::updateViewMatrix()
{
    //viewMatrix = smath::fpsview(camera->pos, camera->rotation.x, camera->rotation.y);
    viewMatrix = glm::lookAt(camera->pos, camera->pos + camera->direction, camera->up);
}

void Renderer::Render()
{
   
    updateViewMatrix();
    
    for (auto& renderable : renderables)
    {
        if (!renderable->mesh.normals.empty())
        {
            renderable->normals.clear();
            renderable->normals = renderable->mesh.normals;
        }
        renderable->vertices.clear();
        renderable->vertices = renderable->mesh.vertices;

        // World space
        transformRenderable(*renderable);
        //-----------------------------

        std::vector<glm::vec4> projectedPoints;
        std::vector<slib::tri> processedFaces;
        std::vector<glm::vec2> screenPoints;
        // TODO: Merge with "transformRenderable"
        createProjectedSpace(*renderable, perspectiveMat, projectedPoints);
    }
    //RenderBuffer();
}


void Renderer::AddRenderable(Renderable& renderable)
{
    renderables.push_back(&renderable);
}

const glm::mat4& Renderer::GetView() const
{
    return viewMatrix;
}

const glm::mat4& Renderer::GetPerspective() const
{
    return perspectiveMat;
}