#ifndef PYRAMID_H
#define PYRAMID_H

#include <string>
#include <glm/glm.hpp>
#include "shader.h"

class Pyramid {
public:
    Pyramid(const std::string& texturePath);
    ~Pyramid();

    void draw(Shader& shader, glm::mat4& transform);

private:
    GLuint VAO, VBO, texture;
    std::string texturePath;

    // Initialize the pyramid (setup VAO, VBO, texture, etc.)
    void init();

    // Load texture
    GLuint loadTexture(const std::string& filename);
    
};

#endif // PYRAMID_H
