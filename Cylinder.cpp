#include "Cylinder.h"
#include <vector>
#include "stb_image.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Cylinder::Cylinder(float baseRadius, float topRadius, float height, int radialDivision, int heightDivision, const std::string& texturePath)
    : texturePath(texturePath), indexCount(0) {
    init(baseRadius, topRadius, height, radialDivision, heightDivision);
}

Cylinder::~Cylinder() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture);
}

void Cylinder::init(float baseRadius, float topRadius, float height, int radialDivision, int heightDivision) {
    std::vector<GLfloat> vertexData;
    std::vector<GLuint> indexData;

    float deltaAngle = 2.0f * M_PI / radialDivision;
    float deltaHeight = height / heightDivision;
    glm::vec2 textureDelta = glm::vec2(1.0f / radialDivision, 1.0f / heightDivision);

    // Generate vertices for the top and bottom circles and the side of the cylinder
    for (int i = 0; i <= heightDivision; ++i) {
        float currentHeight = -0.5f * height + i * deltaHeight;
        float radius = baseRadius + (topRadius - baseRadius) * (currentHeight + 0.5f * height) / height;

        for (int j = 0; j <= radialDivision; ++j) {
            float currentAngle = j * deltaAngle;
            float x = radius * cos(currentAngle);
            float y = currentHeight;
            float z = radius * sin(currentAngle);

            // Position
            vertexData.push_back(x);
            vertexData.push_back(y);
            vertexData.push_back(z);

            // Normal
            glm::vec3 normal = glm::normalize(glm::vec3(x, 0.0f, z));
            vertexData.push_back(normal.x);
            vertexData.push_back(normal.y);
            vertexData.push_back(normal.z);

            // Texture Coordinate
            vertexData.push_back(j * textureDelta.x);
            vertexData.push_back(i * textureDelta.y);
        }
    }

    // Generate indices for the side of the cylinder
    for (int i = 0; i < heightDivision; ++i) {
        for (int j = 0; j < radialDivision; ++j) {
            int first = i * (radialDivision + 1) + j;
            int second = first + radialDivision + 1;

            // First triangle
            indexData.push_back(first);
            indexData.push_back(second);
            indexData.push_back(first + 1);

            // Second triangle
            indexData.push_back(second);
            indexData.push_back(second + 1);
            indexData.push_back(first + 1);
        }
    }
    indexCount = indexData.size();

    // Setup for the VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Texture coordinates attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Load the texture
    texture = loadTexture(texturePath.c_str());
}


void Cylinder::Draw(Shader& shader, const glm::mat4& transform) {
    shader.use();
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader.setInt("texture1", 0);
    // Set other uniforms such as transformation matrices here
    shader.setMat4("model", transform);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

GLuint Cylinder::loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Set texture wrapping and filtering options
    // ...

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}
