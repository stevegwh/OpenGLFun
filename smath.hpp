//
// Created by Steve Wheeler on 23/08/2023.
//

#pragma once
#include <vector>
#include "slib.h"

namespace smath
{
    float getVectorDistance(const glm::vec3& vec);
    glm::vec3 getCentroid(const std::vector<glm::vec3>& points);
    glm::vec3 getCentroid(const slib::tri& t, const std::vector<glm::vec3>& points);
    glm::vec3 facenormal(const slib::tri& t, const std::vector<glm::vec3>& points);
    glm::vec3 axisRotate(const glm::vec3& v, const glm::vec3& u, float angle);
    glm::mat4 view(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up);
    glm::mat4 fpsview( const glm::vec3& eye, float pitch, float yaw );
    void rotateVertex(glm::vec3& v, const glm::vec3& eulerAngles, const glm::vec3& origin);
};

