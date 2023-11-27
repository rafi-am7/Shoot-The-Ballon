//
//  pointLight.h
//  test
//
//  Created by Nazirul Hasan on 22/9/23.
//

#ifndef spotLight_h
#define spotLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

class SpotLight {
public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float cos_theta;
    float k_c;
    float k_l;
    float k_q;

    SpotLight(float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB, float theta, float constant, float linear, float quadratic) {

        position = glm::vec3(posX, posY, posZ);
        direction = glm::vec3(dirX, dirY, dirZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        cos_theta = glm::cos(glm::radians(theta));
        k_c = constant;
        k_l = linear;
        k_q = quadratic;

    }
    void setUpSpotLight(Shader& lightingShader)
    {
        lightingShader.use();


        lightingShader.setVec3("spotLight.position", position);
        lightingShader.setVec3("spotLight.direction", direction);
        
        lightingShader.setVec3("spotLight.ambient", ambientOn * ambient);
        lightingShader.setVec3("spotLight.diffuse", diffuseOn * diffuse);
        lightingShader.setVec3("spotLight.specular", specularOn * specular);
        lightingShader.setFloat("spotLight.cos_theta",  cos_theta);
        lightingShader.setFloat("spotLight.k_c", k_c);
        lightingShader.setFloat("spotLight.k_l", k_l);
        lightingShader.setFloat("spotLight.k_q", k_q);

    }
    void turnOff()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnOn()
    {
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 1.0;
    }
    void turnAmbientOn()
    {
        ambientOn = 1.0;
    }
    void turnAmbientOff()
    {
        ambientOn = 0.0;
    }
    void turnDiffuseOn()
    {
        diffuseOn = 1.0;
    }
    void turnDiffuseOff()
    {
        diffuseOn = 0.0;
    }
    void turnSpecularOn()
    {
        specularOn = 1.0;
    }
    void turnSpecularOff()
    {
        specularOn = 0.0;
    }
private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};

#endif /* pointLight_h */
#pragma once
