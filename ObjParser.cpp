//
// Created by Steve Wheeler on 23/08/2023.
//

#include "ObjParser.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "smath.hpp"

void removeExcessiveWhitespace(const std::string& input, std::string& output)
{
    std::unique_copy (input.begin(), input.end(), std::back_insert_iterator<std::string>(output),
                      [](char a,char b){ return std::isspace(a) && std::isspace(b);});
}


glm::vec3 getVector(const std::string& line)
{
    std::string output;
    removeExcessiveWhitespace(line, output);
    std::stringstream ss(output.erase(0,2));
    std::vector<float> vec;
    vec.reserve(3);
    std::string token;
    while(std::getline(ss, token, ' ')) 
    {
        vec.push_back(std::stof(token));
    }
    return { vec.at(0), vec.at(1), vec.at(2) };
}

glm::vec3 getNormal(const std::string& line)
{
    std::string output;
    removeExcessiveWhitespace(line, output);
    std::stringstream ss(output.erase(0,3));
    std::vector<float> vec;
    vec.reserve(3);
    std::string token;
    while(std::getline(ss, token, ' '))
    {
        vec.push_back(std::stof(token));
    }
    return { vec.at(0), vec.at(1), vec.at(2) };
}

glm::vec2 getTextureVector(const std::string& line)
{
    std::string output;
    removeExcessiveWhitespace(line, output);
    std::stringstream ss(output.erase(0,3));
    std::vector<float> vec;
    vec.reserve(2);
    std::string token;
    while(std::getline(ss, token, ' '))
    {
        vec.push_back(std::stof(token));
        
    }
    return { vec.at(0), vec.at(1) };
}


slib::tri getFace(const std::string& line)
{
    std::string output;
    removeExcessiveWhitespace(line, output);
    std::stringstream ss(output.erase(0,2));
    std::vector<int> verticies;
    std::vector<int> textureCoords;
    std::vector<int> normals;
    std::string token;
    std::string delim = "/";
    while(std::getline(ss, token, ' '))
    {
        token.substr(0, token.find(delim));
        verticies.push_back(std::stoi(token) - 1);
        token.erase(0, token.find(delim) + delim.length());
        
        token.substr(0, token.find(delim));
        textureCoords.push_back(std::stoi(token) - 1);
        token.erase(0, token.find(delim) + delim.length());
        
        token.substr(0, token.find(delim));
        normals.push_back(std::stoi(token) - 1);
        token.erase(0, token.find(delim) + delim.length());
    }
    
    return { 
            verticies.at(0), verticies.at(1), verticies.at(2), 
             textureCoords.at(0), textureCoords.at(1), textureCoords.at(2),
             normals.at(0), normals.at(1), normals.at(2)
         };
}


Mesh* ObjParser::ParseObj(const char *path, const slib::texture& texture)
{
    std::ifstream obj(path);
    if (!obj.is_open())
    {
        std::cout << "Failed to open file" << std::endl;
        exit(1);
    }

    std::vector<glm::vec3> verticies;
    std::vector<glm::vec3> vertexNormals; // Normals stored at same index as the corresponding vertex
    std::vector<glm::vec2> textureCoords;
    std::vector<glm::vec3> normals; // The normals as listed in the obj
    std::vector<slib::tri> faces;
    
    std::string  line;
    while (getline(obj, line))
    {
        if (line[0] == 'v' && line[1] == ' ')
        {
            verticies.push_back(getVector(line));
        }
        else if (line[0] == 'f' && line[1] == ' ')
        {
            faces.push_back(getFace(line));
        }
        else if (line[0] == 'v' && line[1] == 't')
        {
            textureCoords.push_back(getTextureVector(line));
        }
        else if (line[0] == 'v' && line[1] == 'n')
        {
            normals.push_back(getNormal(line));
        }
        
    }

    // TODO: The problem with this approach is it leaves 'vn1, vn2, vn3' in the triangle, which will no longer be
    // necessary.
    vertexNormals.reserve(verticies.size());
    for (const slib::tri& tri : faces) 
    {
        auto n1 = normals.at(tri.vn1);
        auto n2 = normals.at(tri.vn2);
        auto n3 = normals.at(tri.vn3);

        try
        {
            vertexNormals.at(tri.v1) = n1;
        }
        catch (const std::out_of_range& oor) {
            vertexNormals.resize(tri.v1 + 1);
            vertexNormals[tri.v1] = n1;
        }
        try
        {
            vertexNormals.at(tri.v2) = n2;
        }
        catch (const std::out_of_range& oor) {
            vertexNormals.resize(tri.v2 + 1);
            vertexNormals[tri.v2] = n2;
        }
        try
        {
            vertexNormals.at(tri.v3) = n3;
        }
        catch (const std::out_of_range& oor) {
            vertexNormals.resize(tri.v3 + 1);
            vertexNormals[tri.v3] = n3;
        }
    }
    
    
    
    Mesh* mesh = new Mesh(verticies, faces, textureCoords, vertexNormals, texture);
    
    //SDL_free(obj);
    obj.close();
    return mesh;
}