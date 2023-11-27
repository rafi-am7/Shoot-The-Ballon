#include "Pyramid.h"
#include "stb_image.h"
#include <iostream>

Pyramid::Pyramid(const std::string& texturePath)
    :texturePath(texturePath) {
    init();
}

Pyramid::~Pyramid() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Handle texture deletion if necessary
}

void Pyramid::init() {
GLfloat vertices[] = {
    // Positions         // Normals           // Texture Coords
    // Base (y = 0, normal pointing down)
    -0.5f, 0.0f, -0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
     0.5f, 0.0f, -0.5f,  0.0f, -1.0f, 0.0f,  10.0f, 0.0f,
     0.5f, 0.0f,  0.5f,  0.0f, -1.0f, 0.0f,  10.0f, 10.0f,

    0.5f, 0.0f,  0.5f,  0.0f, -1.0f, 0.0f,  10.0f, 10.0f,
    -0.5f, 0.0f,  0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 10.0f,
    -0.5f, 0.0f, -0.5f,  0.0f, -1.0f, 0.0f, 0, 0,

    // Side 1 (Front)
    -0.5f, 0.0f, -0.5f,  0.0f,  0.707f, 0.707f,  0.0f, 0.0f,
     0.5f, 0.0f, -0.5f,  0.0f,  0.707f, 0.707f,  10.0f, 0.0f,
     0.0f, 1.0f,  0.0f,  0.0f,  0.707f, 0.707f,  5, 10.0f,

    // Side 2 (Right)
     0.5f, 0.0f, -0.5f,  0.707f, 0.707f, 0.0f,  0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,  0.707f, 0.707f, 0.0f,  10.0f, 0.0f,
     0.0f, 1.0f,  0.0f,  0.707f, 0.707f, 0.0f,  5, 10.0f,

    // Side 3 (Back)
     0.5f, 0.0f,  0.5f,  0.0f,  0.707f, -0.707f, 0.0f, 0.0f,
    -0.5f, 0.0f,  0.5f,  0.0f,  0.707f, -0.707f, 10.0f, 0.0f,
     0.0f, 1.0f,  0.0f,  0.0f,  0.707f, -0.707f, 5, 10.0f,

    // Side 4 (Left)
    -0.5f, 0.0f,  0.5f, -0.707f, 0.707f, 0.0f,  0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f, -0.707f, 0.707f, 0.0f,  10.0f, 0.0f,
     0.0f, 1.0f,  0.0f, -0.707f, 0.707f, 0.0f,  5, 10.0f,
};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertex normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Load texture
    texture = loadTexture(texturePath);
}

void Pyramid::draw(Shader& shader, glm::mat4& transform) {
    shader.use();
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader.setInt("texture1", 0);

    shader.setMat4("model", transform);

    // Draw the pyramid
    glDrawArrays(GL_TRIANGLES, 0, 18); // 6 triangles (4 sides + base) * 3 vertices each

    glBindVertexArray(0);
}


GLuint Pyramid::loadTexture(const std::string& filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture: " << filename << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}
