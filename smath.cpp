//
// Created by Steve Wheeler on 23/08/2023.
//

#include "smath.hpp"
#include <cmath>
#include "constants.h"


bool compareTrianglesByDepth(const slib::tri& t1, const slib::tri& t2, const std::vector<glm::vec3>& points)
{
    auto c1 = smath::getCentroid({points[t1.v1], points[t1.v2], points[t1.v3]});
    auto c2 = smath::getCentroid({points[t2.v1], points[t2.v2], points[t2.v3]});
    return smath::getVectorDistance(c1) > smath::getVectorDistance(c2);
}

namespace smath
{
    float getVectorDistance(const glm::vec3& vec)
    {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }
    
    glm::vec3 getCentroid(const slib::tri& t, const std::vector<glm::vec3>& points)
    {
        auto t1 = points[t.v1];
        auto t2 = points[t.v2];
        auto t3 = points[t.v3];
        return smath::getCentroid({t1, t2, t3 });
    }

glm::vec3 getCentroid(const std::vector<glm::vec3>& points)
    {
        glm::vec3 result({0, 0, 0});
        for (const auto& v: points) result += v;
        result /= points.size();
        return result;
    }
    
    /*
     * Returns the central normal of the triangle's face.
     */
    glm::vec3 facenormal(const slib::tri& t, const std::vector<glm::vec3>& points)
    {
        glm::vec3 n({0, 0, 0 });
        glm::vec3 a = points[t.v2] - points[t.v1];
        glm::vec3 b = points[t.v3] - points[t.v1];

        n.x = a.y * b.z - a.z * b.y;
        n.y = a.z * b.x - a.x * b.z;
        n.z = a.x * b.y - a.y * b.x;
        
        return glm::normalize(n);
    }
//    void sortVectorsByZ(std::vector<slib::tri>& triangles, const std::vector<slib::vec3>& points) 
//    {
//        std::sort(triangles.begin(), triangles.end(), [&](const slib::tri& t1, const slib::tri& t2) {
//            return compareTrianglesByDepth(t1, t2, points);
//        });
//    }

    glm::vec3 axisRotate(const glm::vec3& v, const glm::vec3& u, float angle)
    {
        angle *= RAD;
        float co = cos(angle);
        float si = sin(angle);
    
        // Extract components of the normalized vector u
        float ux = u.x;
        float uy = u.y;
        float uz = u.z;
    
        // Create the rotation matrix using the formula for arbitrary axis rotation
        const glm::mat3 rotationMatrix(
                { co + ux*ux*(1-co),        ux*uy*(1-co) - uz*si,    ux*uz*(1-co) + uy*si },
                { uy*ux*(1-co) + uz*si,    co + uy*uy*(1-co),       uy*uz*(1-co) - ux*si },
                { uz*ux*(1-co) - uy*si,    uz*uy*(1-co) + ux*si,    co + uz*uz*(1-co)    }
        );
    
        return v * rotationMatrix;
    }


    glm::mat4 view(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
    {
        glm::vec3 zaxis = glm::normalize(eye - target);    // The "forward" vector.
        glm::vec3 xaxis = glm::normalize(cross(up, zaxis));// The "right" vector.
        glm::vec3 yaxis = glm::cross(zaxis, xaxis);     // The "up" vector.

        // Create a 4x4 view matrix from the right, up, forward and eye position vectors
        glm::mat4 viewMatrix(
            {      xaxis.x,            yaxis.x,            zaxis.x,       0 },
            {      xaxis.y,            yaxis.y,            zaxis.y,       0 },
            {      xaxis.z,            yaxis.z,            zaxis.z,       0 },
            { -dot( xaxis, eye ), -dot( yaxis, eye ), -dot( zaxis, eye ),  1 }
        );

        return viewMatrix;
    }

// Pitch must be in the range of [-90 ... 90] degrees and 
// yaw must be in the range of [0 ... 360] degrees.
// Pitch and yaw variables must be expressed in radians.
glm::mat4 fpsview( const glm::vec3& eye, float pitch, float yaw )
{
    pitch *= RAD;
    yaw *= RAD;
    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);
    float cosYaw = cos(yaw);
    float sinYaw = sin(yaw);

    glm::vec3 xaxis = { cosYaw, 0, -sinYaw };
    glm::vec3 yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
    glm::vec3 zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

    // Create a 4x4 view matrix from the right, up, forward and eye position vectors
    glm::mat4 viewMatrix(
                             {       xaxis.x,            yaxis.x,            zaxis.x,      0 },
                             {       xaxis.y,            yaxis.y,            zaxis.y,      0 },
                             {       xaxis.z,            yaxis.z,            zaxis.z,      0 },
                             { -dot( xaxis, eye ), -dot( yaxis, eye ), -dot( zaxis, eye ), 1 }
    );

    return viewMatrix;
}

void rotateVertex(glm::vec3& v, const glm::vec3& eulerAngles, const glm::vec3& origin)
{
    v -= origin;
    const float xrad = eulerAngles.x * RAD;
    const float yrad = eulerAngles.y * RAD;
    const float zrad = eulerAngles.z * RAD;
    const float axc = std::cos(xrad);
    const float axs = std::sin(xrad);
    const float ayc = std::cos(yrad);
    const float ays = -std::sin(yrad);
    const float azc = std::cos(zrad);
    const float azs = -std::sin(zrad);

    // Combined rotation matrix
    glm::mat3 combinedRotationMatrix{
                                         { ayc * azc, ayc * azs, -ays },
                                         { axs * ays * azc - axc * azs, axs * ays * azs + axc * azc, axs * ayc },
                                         { axc * ays * azc + axs * azs, axc * ays * azs - axs * azc, axc * ayc }
                                     };
    glm::vec4 v4 = { v.x, v.y, v.z, 1.0f };
    v = combinedRotationMatrix * v4;
    v += origin;
}
}