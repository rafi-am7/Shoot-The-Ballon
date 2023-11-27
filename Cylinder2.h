#pragma once
#ifndef Cylinder2_h
#define Cylinder2_h

#include <glm/glm.hpp>
#include <string>
#include "shader.h"

class Cylinder2 {
public:
    Cylinder2(float baseRadius, float topRadius, float height, int radialDivision, int heightDivision, const std::string& texturePath);
    ~Cylinder2();

    void Draw(Shader& shader, const glm::mat4& transform);

private:
    GLuint VAO, VBO, EBO, texture;
    std::string texturePath;
    int indexCount;
    void init(float baseRadius, float topRadius, float height, int radialDivision, int heightDivision);
    GLuint loadTexture(const char* path);
};

#endif // CYLINDER_H
