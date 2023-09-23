#pragma once
#include <utility>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


namespace slib
{
    struct texture
    {
        int w, h;
        std::vector<unsigned char> data;
        unsigned int bpp;
    };

    texture DecodePng(const char* filename);
    
    struct Frustum;
    
    struct Camera
    {
        glm::vec3 pos;
        glm::vec3 rotation;
        glm::vec3 direction;
        glm::vec3 up;
        const float zFar;
        const float zNear;
        Camera(glm::vec3 _pos, glm::vec3 _rotation, glm::vec3 _direction, glm::vec3 _up, float _zFar, float _zNear)
            :
            pos(_pos), rotation(_rotation), direction(_direction), up(_up), zFar(_zFar), zNear(_zNear)
        {};
        void Rotate(float x, float y);
    };
    
    struct Frustum
    {
        const float nearW;
        const float nearH;
        const float farW;
        const float farH;
        float near{};
        float far{};
        Frustum(float _nearW, float _nearH, float _farW, float _farH)
            :
            nearW(_nearW), nearH(_nearH), farW(_farW), farH(_farH)
        {};
    };
    
    /*
     * Contains the index of the relevant vertices in the mutable vertices array of the object.
     */
    struct tri
    {
        const int v1, v2, v3;
        const int vt1, vt2, vt3;
        const int vn1, vn2, vn3; // these should be ignored now
    };
    
    struct Color
    {
        int r, g, b;
    };
}
