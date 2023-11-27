//
//  main.cpp
//  3D Object Drawing
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionLight.h"
#include "sphere.h"
#include "spotLight.h"
#include "cube.h"
#include "stb_image.h"
#include "cylinder.h"
#include "Sphere2.h"
#include "Cylinder2.h"
#include "Pyramid.h"

#include <iostream>
#include <random>

#include <stdlib.h>
#include<windows.h>  

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void tree(Pyramid& pyramid, Shader shader, glm::mat4 model1);
glm::mat4 transforamtion(float tx, float ty, float tz, float sx, float sy, float sz);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);

unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector<float>& coordinates, vector<float>& normals, vector<int>& indices, vector<float>& vertices);
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L);
long long nCr(int n, int r);
void read_file(std::string file_name, std::vector<float>& vec);
void drawCurve(Shader ourShader, glm::mat4 moveMatrix, unsigned int VAO, vector<int> indices, glm::vec4 color, float rotateAngleTest_Y);
void FerrisWheel(Shader ourShader, glm::mat4 moveMatrix);
void FerrisWheelSeat(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Z=0);
void load_texture(unsigned int& texture, string image_name, GLenum format);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
float run_speed = 0.004f;
//initial values
float sky_angle = 0.0f;
float lower_lim = 0.0f;
float upper_lim = 0.0f;
float ferrisWheelAngle = 0.0f;
float ferrisWheelSpeed = 0.02f;
float ferrisIncrement = 0.012f;
bool isFerrisWheelOn = true;
unsigned int texture0, texture1, texture2, texture3, texture4, texture5, texture6, texture7, texture8, texture9;



float broken_balloonY = 0.0f;
bool balloon_done = false;
float balloon_doneX, balloon_doneY, balloon_doneZ, bbx=0.0f,bby=0.0f,bbz=0.0f;


float gun_x=0.0f;
float gun_y=0.7f;

float bullet_x;
float bullet_y;
float slope_Y;
float slope_X;
int score = 5;
float balloon_x=1.5f;
float balloon_y = 6.0f;
float balloon_z;
float bz = 0.0f;

float up_angle = 0.0f;
float right_angle = 0.0f;

bool first_block = true;
bool balloon = true;
bool pause = false;
// camera
float camera_z = 0.0f;
Camera camera(glm::vec3(0.0f, 1.0f, -6.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 10.0, eyeZ = 1.0;
float lookAtX = 0.0, lookAtY = 10.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);
SpotLight spotlight(
    0.0f,1.0f,-3.0f,
    0.0f, -1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    30.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);


DirectionLight directlight(
    0.0f, 1.0f, 0.0f,
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);
vector <float> cntrlPoints, cntrlPointsBoat, cntrlPointsRotor, cntrlPointsCylinder, cntrlPointsCarousal, cntrlPointsHead;
vector <float> coordinates, coordinatesBoat, coordinatesRotor, coordinatesCylinder, coordinatesCarousal, coordinatesHead;
vector <float> normals, normalsBoat, normalsRotor, normalsCylinder, normalsCarousal, normalsHead;
vector <int> indices, indicesBoat, indicesRotor, indicesCylinder, indicesCarousal, indicesHead;
vector <float> vertices, verticesBoat, verticesRotor, verticesCylinder, verticesCarousal, verticesHead;

const double pi = 3.14159265389;
const int nt = 40;
const int ntheta = 30;

unsigned int cubeVAO, cubeVBO, cubeEBO;
unsigned int bezierVAO, boatVAO, rotorVAO, cylinderVAO, carousalVAO, headVAO;




// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(5.5f, 3.0f, 0.0f),
    glm::vec3(5.5f, 3.0f, 10.0f),
    glm::vec3(-5.5f, 3.0f, 0.0f),
    glm::vec3(-5.5f, 3.0f, 10.0f),
    glm::vec3(5.5f, 3.0f, -15.0f),
    glm::vec3(5.5f, 3.0f, -20.0f),
    glm::vec3(-5.5f, 3.0f, -21.0f),
    glm::vec3(-5.5f, 3.0f, -25.0f),
    glm::vec3(2.0f, 0.0f, -15.0f),
    glm::vec3(2.0f, 0.0f, -1.0f),
    glm::vec3(2.5f, 0.0f, 0.0f)

};
glm::vec3 objectPositions[] = {
    glm::vec3(1.50f,  1.50f,  0.0f),
    glm::vec3(1.5f,  1.5f,  10.0f),
    glm::vec3(-1.5f,  1.5f,  0.0f),
    glm::vec3(-1.5f,  1.5f,  10.0f)
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);
PointLight pointlight5(

    pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    5       // light number
);
PointLight pointlight6(

    pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    6       // light number
);
PointLight pointlight7(

    pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    7      // light number
);
PointLight pointlight8(

    pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    8       // light number
);
PointLight pointlight9(

    pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    9      // light number
);
PointLight pointlight10(

    pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    10   // light number
);
PointLight pointlight11(

    pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    11      // light number
);
int numm = 0;
// light settings
bool pointLightOn = true;
bool directionLighton = true;
bool spotlighton = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool shoot = false;
float bullet_z = -5.2f;
// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

class Balloon
{
public:
    float speed;
    float x=0.0,y=6.0,z=0.0;
    Sphere bb = Sphere();
};
void func(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Balloon b[])
{
    float baseHeight = 0.1;
    float width = 20;
    float length = 35;

    //base-1
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.5, 0.3);


    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;


    //wall
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5f, 0.0f, 5.0f));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.1f, 5.0f, 0.1f));
    //model = alTogether * translateMatrix * scaleMatrix;
    //lightingShader.setMat4("model", model);
    //drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0);

    

    //gun
    Cylinder gun = Cylinder();
    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x, gun_y, -5.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.8f, 0.05f));
    glm::mat4 rotUp = glm::rotate(identityMatrix, glm::radians(up_angle-90.0f), glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 rotRight = glm::rotate(identityMatrix, glm::radians(right_angle), glm::vec3(0.0, 0.0, 1.0));
    model = alTogether * translateMatrix * rotUp * rotRight * scaleMatrix;
    gun.drawCylinder(lightingShader, model, 1.0, 1.0, 1.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x-0.025, gun_y, -5.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.8f, 0.05f));
    rotUp = glm::rotate(identityMatrix, glm::radians(up_angle - 90.0f), glm::vec3(1.0, 0.0, 0.0));
    rotRight = glm::rotate(identityMatrix, glm::radians(right_angle), glm::vec3(0.0, 0.0, 1.0));
    model = alTogether * translateMatrix * rotUp * rotRight * scaleMatrix;
    gun.drawCylinder(lightingShader, model, 1.0, 1.0, 1.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x-0.05, gun_y - 0.05, -5.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.07f, 0.07f, 0.35f));
    rotUp = glm::rotate(identityMatrix, glm::radians(up_angle), glm::vec3(1.0, 0.0, 0.0));
    rotRight = glm::rotate(identityMatrix, glm::radians(right_angle), glm::vec3(0.0, 1.0, 0.0));
    model = alTogether * translateMatrix * rotUp * rotRight * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 1.0, 0.0, 0.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x - 0.075, gun_y+0.025, -5.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.3f, 0.1f));
    rotUp = glm::rotate(identityMatrix, glm::radians(up_angle-180.0f), glm::vec3(1.0, 0.0, 0.0));
    rotRight = glm::rotate(identityMatrix, glm::radians(-right_angle), glm::vec3(0.0, 1.0, 0.0));
    model = alTogether * translateMatrix * rotUp * rotRight * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 1.0, 0.8, 0.9);
    float bx = 0.0, by = 0.0;

    //shoot
    if (shoot)
    {
        bz += 0.1f;
        bx = slope_X * bz;
        by = slope_Y * bz;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(bullet_x + bx, bullet_y + by, bullet_z + bz));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
        model = alTogether * translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 0.5);
    }

    float bux = bullet_x + bx, buy = bullet_y + by, buz = bullet_z + bz;



    for (int i = 0; i < 5; i++)
    {
        float distance = sqrt((b[i].x - bux) * (b[i].x - bux) + (b[i].y - buy) * (b[i].y - buy) + (b[i].z - buz) * (b[i].z - buz));

        if (distance <= 0.6)
        {
            numm++;
            if (numm >= 5)
            {
                lower_lim += 0.001;
                numm = 0;
            }
            score++;
            balloon_done = true;
            balloon_doneX = b[i].x;
            balloon_doneY = b[i].y;
            balloon_doneZ = b[i].z;
            shoot = false;
            bullet_z = -5.2f;
            bz = 0.0;
            std::random_device rd;  // Seed for the random number engine
            std::mt19937 gen(rd());  // Mersenne Twister engine
            std::uniform_real_distribution<float> dis(4.0f, 6.0f);
            b[i].z = dis(gen);
            std::uniform_real_distribution<float> dis2(i * 1.2 - 3, i * 1.2 - 3 + 0.6);
            b[i].x = dis2(gen);
            std::uniform_real_distribution<float> dis3(0.001+lower_lim, 0.003+lower_lim);
            b[i].speed = dis3(gen);
            b[i].y = 6.0;
        }
    }


    if (bullet_z + bz > 7.0f)
    {
        shoot = false;
        bullet_z = -5.2f;
        bz = 0.0;
    }

    if (balloon_done)
    {
        bbx += 0.1f;

        bby += 0.1f;
        bbz += 0.1f;

        //right
        translateMatrix = glm::translate(identityMatrix, glm::vec3(balloon_doneX+bbx, balloon_doneY, balloon_doneZ));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
        glm::mat4 fataModel = alTogether * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, fataModel, 1.0, 0.0, 0.0);
        //left
        translateMatrix = glm::translate(identityMatrix, glm::vec3(balloon_doneX - bbx, balloon_doneY, balloon_doneZ));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
        fataModel = alTogether * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, fataModel, 1.0, 0.0, 0.0);
        //up
        translateMatrix = glm::translate(identityMatrix, glm::vec3(balloon_doneX, balloon_doneY+bby, balloon_doneZ));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
        fataModel = alTogether * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, fataModel, 1.0, 0.0, 0.0);
        //down
        translateMatrix = glm::translate(identityMatrix, glm::vec3(balloon_doneX, balloon_doneY - bby, balloon_doneZ));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
        fataModel = alTogether * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, fataModel, 1.0, 0.0, 0.0);

        //right up
        translateMatrix = glm::translate(identityMatrix, glm::vec3(balloon_doneX+bbx, balloon_doneY + bby, balloon_doneZ+bbz));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
        fataModel = alTogether * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, fataModel, 1.0, 0.0, 0.0);

        //left up
        translateMatrix = glm::translate(identityMatrix, glm::vec3(balloon_doneX - bbx, balloon_doneY + bby, balloon_doneZ + bbz));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
        fataModel = alTogether * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, fataModel, 1.0, 0.0, 0.0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(balloon_doneX - bbx, balloon_doneY - bby, balloon_doneZ + bbz));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
        fataModel = alTogether * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, fataModel, 1.0, 0.0, 0.0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(balloon_doneX + bbx, balloon_doneY - bby, balloon_doneZ + bbz));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
        fataModel = alTogether * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, fataModel, 1.0, 0.0, 0.0);

    }

    if (bbx >= 3.0f)
    {
        bbx = 0.0;
        bby = 0.0;
        bbz = 0.0;
        balloon_done = false;
    }




}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    read_file("ferris_wheel_points.txt", cntrlPoints);
    bezierVAO = hollowBezier(cntrlPoints.data(), ((unsigned int)cntrlPoints.size() / 3) - 1, coordinates, normals, indices, vertices);
    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    load_texture(texture1, "grass_2.png", GL_RGBA);
    load_texture(texture2, "brickwall_1.jpg", GL_RGB);
    load_texture(texture3, "brickwall_2.jpg", GL_RGB);
    load_texture(texture4, "road.jpg", GL_RGB);
    load_texture(texture5, "concrete1.png", GL_RGBA);
    load_texture(texture6, "concrete2.png", GL_RGBA);
    load_texture(texture7, "concrete3.png", GL_RGBA);
    load_texture(texture8, "leather.png", GL_RGBA);
    load_texture(texture9, "leather2.png", GL_RGBA);
    //unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sphere = Sphere();



    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");

    string diffuseMapPath = "grass.jpg";
    string specularMapPath = "grass.jpg";

    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath2 = "game_over.jpg";
    string specularMapPath2 = "game_over.jpg";
    unsigned int diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube2 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

     diffuseMapPath2 = "sky.jpg";
     specularMapPath2 = "sky.jpg";
      diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
      specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 sky = Sphere2(1.0f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffMap2, specMap2, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathnum = "0.png";
    string specularMapPathnum = "0.png";
    unsigned int diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube zero = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPathnum = "1.png";
    specularMapPathnum = "1.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube one = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "2.png";
    specularMapPathnum = "2.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube two = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "3.png";
    specularMapPathnum = "3.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube three = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "4.png";
    specularMapPathnum = "4.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube four = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "5.png";
    specularMapPathnum = "5.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube five = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "6.png";
    specularMapPathnum = "6.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube six = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "7.png";
    specularMapPathnum = "7.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube seven = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "8.png";
    specularMapPathnum = "8.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube eight = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "9.png";
    specularMapPathnum = "9.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube nine = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    Cube score_block[10] = { zero,one,two,three,four,five,six,seven,eight,nine };
    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------
    std::random_device rd;  // Seed for the random number engine
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_real_distribution<float> dis(4.0f, 7.0f);
    float randomValue = dis(gen);
    balloon_z = randomValue;

    Balloon b[5];
    for (int i=0;i<5;i++)
    {
        //cout << "here" << endl;

        b[i].y = 6.0f;
        std::random_device rd;  // Seed for the random number engine
        std::mt19937 gen(rd());  // Mersenne Twister engine
        std::uniform_real_distribution<float> dis(4.0f, 6.0f);
        b[i].z = dis(gen);
        std::uniform_real_distribution<float> dis2(i * 1.2 - 3, i * 1.2 - 3 + 0.6);
        b[i].x = dis2(gen);
        std::uniform_real_distribution<float> dis3(0.001+lower_lim, 0.003+lower_lim);
        b[i].speed = dis3(gen);
    }
    
    Pyramid pyramid("tree.jpg");
    glm::mat4 modelPyr = glm::mat4(1.0f);
    Cylinder2 cylinder2(.15, .1, 1, 16, 20, "treebase.png");

    cout << camera.Position[0] << " " << camera.Position[1] << " " << camera.Position[2];
    camera.Position[2] = -8;
    int count = 0;
    while (!glfwWindowShouldClose(window))
    {
        
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //camera.Position = glm::vec3(0.0f, 1.0f, -7.5f);

        

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        pointlight5.setUpPointLight(lightingShader);

        pointlight6.setUpPointLight(lightingShader);

        pointlight7.setUpPointLight(lightingShader);

        pointlight8.setUpPointLight(lightingShader);
        pointlight9.setUpPointLight(lightingShader);
        pointlight10.setUpPointLight(lightingShader);
        pointlight11.setUpPointLight(lightingShader);

        directlight.setUpDirectionLight(lightingShader);

        spotlight.setUpSpotLight(lightingShader);


        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model,model2;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        func(cubeVAO, lightingShader, model,b);



        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //balloon_y -= 0.001f;
        glm::mat4 modelForSphere = glm::mat4(1.0f);
        //translateMatrix = glm::translate(model, glm::vec3(balloon_x, balloon_y, balloon_z));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8, 1.0, 1.0));
        //modelForSphere = translateMatrix * scaleMatrix;
        //sphere.drawSphere(lightingShader, modelForSphere);


        //if (balloon_y <= -1.0f)
        //{
        //    balloon_y = 6.0f;
        //    std::random_device rd;  // Seed for the random number engine
        //    std::mt19937 gen(rd());  // Mersenne Twister engine
        //    std::uniform_real_distribution<float> dis(4.0f, 6.0f);
        //    balloon_z = dis(gen);
        //    std::uniform_real_distribution<float> dis2(-1.0f, 1.0f);
        //    balloon_x = dis2(gen);
        //}

        //balloon 1
        for (int i=0;i<5;i++)
        {
            //cout << "draw" << endl;
            if(!pause)
                b[i].y -= b[i].speed;
            translateMatrix = glm::translate(model, glm::vec3(b[i].x, b[i].y, b[i].z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8, 1.0, 1.0));
            modelForSphere = translateMatrix * scaleMatrix;
            b[i].bb.drawSphere(lightingShader, modelForSphere);

            if (b[i].y <= 0.0f)
            {
                numm++;
                if (numm >= 15)
                {
                    lower_lim += 0.0005;
                    numm = 0;
                }
                score--;
                b[i].y = 6.0f;
                std::random_device rd;  // Seed for the random number engine
                std::mt19937 gen(rd());  // Mersenne Twister engine
                std::uniform_real_distribution<float> dis(4.0f, 6.0f);
                b[i].z = dis(gen);
                std::uniform_real_distribution<float> dis2(i * 1.2 - 3, i * 1.2 - 3 + 0.6);
                b[i].x = dis2(gen);
                std::uniform_real_distribution<float> dis3(0.001+lower_lim, 0.003+lower_lim);
                b[i].speed = dis3(gen);
            }
        }
        


        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        float cr, cg, cb;
        if (pointLightOn)
        {
            cr = 0.8f;
            cg = 0.8f;
            cb = 0.8f;
        }
        else
        {
            cr = 0.0f;
            cg = 0.0f;
            cb = 0.0f;
        }
        // point lights
        glBindVertexArray(lightCubeVAO);
        model2=model;
        for (unsigned int i = 0; i < 8; i++)
        {
            model2 = glm::mat4(1.0f);
            model2 = glm::translate(model2, pointLightPositions[i]);
            model2 = glm::scale(model2, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model2);
            ourShader.setVec3("color", glm::vec3(cr, cg, cb));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        lightingShaderWithTexture.use();
        directlight.setUpDirectionLight(lightingShaderWithTexture);
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);
        pointlight5.setUpPointLight(lightingShaderWithTexture);
        pointlight6.setUpPointLight(lightingShaderWithTexture);
        pointlight7.setUpPointLight(lightingShaderWithTexture);
        pointlight8.setUpPointLight(lightingShaderWithTexture);
        pointlight9.setUpPointLight(lightingShaderWithTexture);
        pointlight10.setUpPointLight(lightingShaderWithTexture);
        pointlight11.setUpPointLight(lightingShaderWithTexture);

        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, objectPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            //ourShader.setMat4("model", model);
            //ourShader.setVec3("color", glm::vec3(cr, cg, cb));
            //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //ferrisqheel
        glm::mat4 identityMatrixferris = glm::mat4(1.0f);
        glm::mat4 translateMatrixFerris = glm::translate(identityMatrixferris, glm::vec3(1.0f, 3.0f, -15.0f));
        FerrisWheel(lightingShaderWithTexture, translateMatrixFerris);
        
        //tree
      //  for (int i = 0; i < 25; i += 5) {
            modelPyr =  transforamtion( -6, -0.6, -25, 1, 1, 1);
            modelPyr = modelPyr * model;
            tree(pyramid, lightingShaderWithTexture, modelPyr);
            modelPyr = transforamtion( -6*5, -4.0, -125, 3, 5, 3);
            modelPyr = model * modelPyr;
            cylinder2.Draw(lightingShaderWithTexture, modelPyr);
       // }

            modelPyr = transforamtion(-7, -0.6, -20, 1, 1, 1);
            modelPyr = modelPyr * model;
            tree(pyramid, lightingShaderWithTexture, modelPyr);
            modelPyr = transforamtion(-7*5, -4.0, -100, 3, 5, 3);
            modelPyr = model * modelPyr;
            cylinder2.Draw(lightingShaderWithTexture, modelPyr);


            modelPyr = transforamtion(-3, -0.6, -23, 1, 1, 1);
            modelPyr = modelPyr * model;
            tree(pyramid, lightingShaderWithTexture, modelPyr);
            modelPyr = transforamtion(-3 * 5, -4.0, -5*23, 3, 5, 3);
            modelPyr = model * modelPyr;
            cylinder2.Draw(lightingShaderWithTexture, modelPyr);
            
            float x = -4.0f, z = -21.0f;
            modelPyr = transforamtion(x, -0.6, z, 1, 1, 1);
            modelPyr = modelPyr * model;
            tree(pyramid, lightingShaderWithTexture, modelPyr);
            modelPyr = transforamtion(x*5 , -4, 5 *z, 3, 5, 3);
            modelPyr = model * modelPyr;
            cylinder2.Draw(lightingShaderWithTexture, modelPyr);

            x = -0.4f, z = -23.0f;
            modelPyr = transforamtion(x, -0.6, z, 1, 1, 1);
            modelPyr = modelPyr * model;
            tree(pyramid, lightingShaderWithTexture, modelPyr);
            modelPyr = transforamtion(x * 5, -4, 5 * z, 3, 5, 3);
            modelPyr = model * modelPyr;
            cylinder2.Draw(lightingShaderWithTexture, modelPyr);

            x = -9.0f, z = -13.0f;
            modelPyr = transforamtion(x, -0.6, z, 1, 1, 1);
            modelPyr = modelPyr * model;
            tree(pyramid, lightingShaderWithTexture, modelPyr);
            modelPyr = transforamtion(x * 5, -4, 5 * z, 3, 5, 3);
            modelPyr = model * modelPyr;
            cylinder2.Draw(lightingShaderWithTexture, modelPyr);

            x = -0.0f, z = -30.0f;
            modelPyr = transforamtion(x, -0.6, z, 1, 1, 1);
            modelPyr = modelPyr * model;
            tree(pyramid, lightingShaderWithTexture, modelPyr);
            modelPyr = transforamtion(x * 5, -4, 5 * z, 3, 5, 3);
            modelPyr = model * modelPyr;
            cylinder2.Draw(lightingShaderWithTexture, modelPyr);
        //grass
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-10.5f, 0.1f, -8.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(25.0f, 0.2f, 25.0f));
        glm::mat4 modelMatrixForContainer3 = translateMatrix * scaleMatrix;
        cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-35.5f, 0.1f, -8.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(25.0f, 0.2f, 25.0f));
        modelMatrixForContainer3 = translateMatrix * scaleMatrix;
        cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(14.5f, 0.1f, -8.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(25.0f, 0.2f, 25.0f));
        modelMatrixForContainer3 = translateMatrix * scaleMatrix;
        cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-10.5f, 0.1f, -33.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(25.0f, 0.2f, 25.0f));
        modelMatrixForContainer3 = translateMatrix * scaleMatrix;
        cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-35.5f, 0.1f, -33.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(25.0f, 0.2f, 25.0f));
        modelMatrixForContainer3 = translateMatrix * scaleMatrix;
        cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(14.5f, 0.1f, -33.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(25.0f, 0.2f, 25.0f));
        modelMatrixForContainer3 = translateMatrix * scaleMatrix;
        cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);


        int A[3];
        A[0] = 0;
        A[1] = 0;
        A[2] = 0;
        int ff = score;
        int inddd = 2;
        while (ff > 0) {
            A[inddd] = ff % 10;
            ff /= 10;
            inddd--;
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0, 5.0f, 20.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 5.0f, 0.05f));
        glm::mat4 modelForScore1 = model * translateMatrix * scaleMatrix;
        score_block[A[0]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore1);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 5.0f, 20.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 5.0f, 0.05f));
        glm::mat4 modelForScore2 = model * translateMatrix * scaleMatrix;
        score_block[A[1]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore2);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.0, 5.0f, 20.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 5.0f, 0.05f));
        glm::mat4 modelForScore3 = model * translateMatrix * scaleMatrix;
        score_block[A[2]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore3);




        if (score == 0)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-17.0, -10.0f, -45.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(50.0f, 30.0f, 25.0f));
            glm::mat4 modelForGameOver = model * translateMatrix * scaleMatrix;
            cube2.drawCubeWithTexture(lightingShaderWithTexture, modelForGameOver);
            pause = true;
        }

        if (sky_angle >= 359.0)
            sky_angle = 0.0f;
        else
            sky_angle += 0.1f;

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0f, -25.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(200.0f, 200.0f, 200.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(sky_angle), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 modelForSky = model * translateMatrix * rotateYMatrix * scaleMatrix;
        sky.drawSphereWithTexture(lightingShaderWithTexture, modelForSky);

        //translateMatrix = glm::translate(identityMatrix, glm::vec3(2.05f, 0.1f, grass2));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 10.0f));
        //glm::mat4 modelMatrixForContainer4 = translateMatrix * scaleMatrix;
        //cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer4);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
glm::mat4 transforamtion(float tx, float ty, float tz, float sx, float sy, float sz) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
    model = translateMatrix * scaleMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix;
    return model;
}

void load_texture(unsigned int& texture, string image_name, GLenum format)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(image_name.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture " << image_name << endl;
    }
    stbi_image_free(data);
}

void read_file(std::string file_name, std::vector<float>& vec)
{
    ifstream file(file_name);
    float number;

    while (file >> number)
        vec.push_back(number);

    file.close();
}

long long nCr(int n, int r)
{
    if (r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

//polynomial interpretation for N points
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i < L + 1; i++)
    {
        long long ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i * 3];
        y += coef * ctrlpoints[(i * 3) + 1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector<float>& coordinates, vector<float>& normals, vector<int>& indices, vector<float>& vertices)
{
    int i, j;
    float x, y, z, r;                //current coordinates
    float theta;
    float nx, ny, nz, lengthInv;    // vertex normal


    const float dtheta = 2 * pi / ntheta;        //angular step size

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];

    for (i = 0; i <= nt; ++i)              //step through y
    {
        BezierCurve(t, xy, ctrlpoints, L);
        r = xy[0];
        y = xy[1];
        theta = 0;
        t += dt;
        lengthInv = 1.0 / r;

        for (j = 0; j <= ntheta; ++j)
        {
            double cosa = cos(theta);
            double sina = sin(theta);
            z = r * cosa;
            x = r * sina;

            coordinates.push_back(x);
            coordinates.push_back(y);
            coordinates.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            // center point of the circle (0,y,0)
            nx = (x - 0) * lengthInv;
            ny = (y - y) * lengthInv;
            nz = (z - 0) * lengthInv;

            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            theta += dtheta;
        }
    }

    // generate index list of triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1

    int k1, k2;
    for (int i = 0; i < nt; ++i)
    {
        k1 = i * (ntheta + 1);     // beginning of current stack
        k2 = k1 + ntheta + 1;      // beginning of next stack

        for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
        {
            // k1 => k2 => k1+1
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            // k1+1 => k2 => k2+1
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }

    size_t count = coordinates.size();
    for (int i = 0; i < count; i += 3)
    {
        vertices.push_back(coordinates[i]);
        vertices.push_back(coordinates[i + 1]);
        vertices.push_back(coordinates[i + 2]);

        vertices.push_back(normals[i]);
        vertices.push_back(normals[i + 1]);
        vertices.push_back(normals[i + 2]);
    }

    unsigned int bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    // create VBO to copy vertex data to VBO
    unsigned int bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        vertices.data(),   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage

    // create EBO to copy index data
    unsigned int bezierEBO;
    glGenBuffers(1, &bezierEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        (unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
        indices.data(),               // ptr to index data
        GL_STATIC_DRAW);                   // usage

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set attrib arrays with stride and offset
    int stride = 24;     // should be 24 bytes
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

    // unbind VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return bezierVAO;
}

void drawCurve(Shader ourShader, glm::mat4 moveMatrix, unsigned int VAO, vector<int> indices, glm::vec4 color, float rotateAngleTest_Y)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateYMatrix;

    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Y), glm::vec3(0.0f, 1.0f, 0.0f));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
    model = translateMatrix * rotateYMatrix * scaleMatrix;

    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}



void FerrisWheel(Shader ourShader, glm::mat4 moveMatrix)
{
    ourShader.use();
    //front
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrix, rotateTemp;

    if (isFerrisWheelOn)
    {
        ferrisWheelSpeed += ferrisIncrement;
        ferrisWheelSpeed = min(2.5f, ferrisWheelSpeed);
    }
    else
    {
        ferrisWheelSpeed -= ferrisIncrement;
        ferrisWheelSpeed = max(0.02f, ferrisWheelSpeed);
    }

    if (ferrisWheelSpeed != 0.02f)
        ferrisWheelAngle += ferrisWheelSpeed;

    ferrisWheelAngle = fmod(ferrisWheelAngle, 360);
    rotateTemp = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(ferrisWheelAngle), glm::vec3(0.0f, 0.0f, 1.0f));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 2.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
    model = translateMatrix * rotateZMatrix * scaleMatrix * rotateTemp;

    glm::vec4 color = glm::vec4(48.0 / 256, 45.0 / 256, 181.0 / 256, 1.0f);
    //glm::vec4 color = glm::vec4(1.0f);
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);
    glBindVertexArray(bezierVAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
    //bars
    glm::mat4 translateToPivot, translateFromPivot;
    glm::vec4 translateToPivotOrg, translateFromPivotOrg;
    glm::vec4 pivot = glm::vec4(glm::vec3(0.0f), 1.0f);

    pivot = translateMatrix * pivot;

    translateToPivot = glm::translate(identityMatrix, glm::vec3(-pivot));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(pivot));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 2.0f, -0.025f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.4f, 0.1f));
    for (float i = 0.0f; i >= -360.0f; i -= 45.0f)
    {
        rotateTemp = glm::rotate(identityMatrix, glm::radians(i), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * rotateTemp * scaleMatrix;
        ourShader.setMat4("model", moveMatrix * model);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }



    //back
    rotateTemp = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(ferrisWheelAngle), glm::vec3(0.0f, 0.0f, 1.0f));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 2.0f, -1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
    model = translateMatrix * rotateZMatrix * scaleMatrix * rotateTemp;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(bezierVAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
    //bars

    translateToPivot = glm::translate(identityMatrix, glm::vec3(-pivot));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(pivot));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 2.0f, -1.025f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.4f, 0.1f));
    for (float i = 0.0f; i >= -360.0f; i -= 45.0f)
    {
        rotateTemp = glm::rotate(identityMatrix, glm::radians(i), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * rotateTemp * scaleMatrix;
        ourShader.setMat4("model", moveMatrix * model);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }


    //connecting bars
    //middle
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.975f, 1.975f, -1.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 1.7f));
    model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //top ones
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 3.5f, -1.005f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 1.7f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    for (float i = 0.0f; i >= -360.0f; i -= 90.0f)
    {
        rotateTemp = glm::rotate(identityMatrix, glm::radians(i), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateFromPivot * rotateZMatrix * translateToPivot * translateFromPivot * rotateTemp * translateToPivot * translateMatrix * scaleMatrix;
        ourShader.setMat4("model", moveMatrix * model);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    //pillars
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, -4.0f, 0.1f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-18.3f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.1f, -4.0f, 0.1f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(18.3f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, -4.0f, -1.25f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-18.3f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.1f, -4.0f, -1.25f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(18.3f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    color = glm::vec4(1.0f);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color);
    //sitting places
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    //FerrisWheelSeat(ourShader, moveMatrix*translateMatrix, ferrisWheelAngle);
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 1.5f, 0.0f));
    //FerrisWheelSeat(ourShader, moveMatrix*translateMatrix, ferrisWheelAngle);
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 3.0f, 0.0f));
    //FerrisWheelSeat(ourShader,moveMatrix*translateMatrix, ferrisWheelAngle);
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f, 1.5f, 0.0f));
    //FerrisWheelSeat(ourShader, moveMatrix*translateMatrix, ferrisWheelAngle);


}

void FerrisWheelSeat(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Z)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrixLocal, rotateZMatrixMain, rotateTemp;
    glm::mat4 translateToPivotMain, translateFromPivotMain;
    glm::vec4 translateToPivotOrgMain, translateFromPivotOrgMain;
    glm::mat4 translateToPivotLocal, translateFromPivotLocal;
    glm::vec4 translateToPivotOrgLocal, translateFromPivotOrgLocal;
    glm::vec4 pivot1, pivot2;
    glm::vec4 mainPivotPoint = glm::vec4(3.0f, 2.0f, 0.0f, 1.0);
    glm::vec4 localPivotPoint = glm::vec4(2.975f, 0.475f, -1.005f, 1.0);

    pivot1 = mainPivotPoint;
    translateToPivotMain = glm::translate(identityMatrix, glm::vec3(-pivot1));
    translateFromPivotMain = glm::translate(identityMatrix, glm::vec3(pivot1));
    rotateZMatrixMain = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Z), glm::vec3(0.0f, 0.0f, 1.0f));

    pivot2 = translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * moveMatrix * localPivotPoint;
    translateToPivotLocal = glm::translate(identityMatrix, glm::vec3(-pivot2));
    translateFromPivotLocal = glm::translate(identityMatrix, glm::vec3(pivot2));
    rotateZMatrixLocal = glm::rotate(identityMatrix, glm::radians(-rotateAngleTest_Z), glm::vec3(0.0f, 0.0f, 1.0f));

    glBindTexture(GL_TEXTURE_2D, texture3);
    //base
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, -0.2f, -0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.1f, 1.5f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, texture8);
    //left
    //seat
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, -0.2f, -0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.5f, 1.5f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, -0.2f, -0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 1.5f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    //seat right
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.2f, -0.2f, -0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.5f, 1.5f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.45f, -0.2f, -0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 1.5f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //top bars
    glBindTexture(GL_TEXTURE_2D, texture0);
    //left
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.25f, -0.8f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1f, 0.1f, 0.1f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.25f, -0.3f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1f, 0.1f, 0.1f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.25f, -0.899f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.15f, 0.1f, 0.05f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.25f, -0.1751f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.15f, 0.1f, 0.05f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    //right
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.25f, -0.8f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.1f, 0.1f, 0.1f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.25f, -0.3f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.1f, 0.1f, 0.1f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.25f, -0.899f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.15f, 0.1f, 0.05f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.25f, -0.1751f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.15f, 0.1f, 0.05f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


}

//tree
void tree(Pyramid& pyramid, Shader shader, glm::mat4 model1) {
    glm::mat4 modeltp, identityMatrixtp, rotateYMatrixtp, goaroundtp;
    for (int i = 0; i < 360; i += 30) {

        identityMatrixtp = glm::mat4(1.0f);
        goaroundtp = glm::rotate(identityMatrixtp, glm::radians(float(i)), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateYMatrixtp = glm::rotate(identityMatrixtp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modeltp = model1 * transforamtion(0, 1, 0, 2, 2, 2);
        modeltp *= goaroundtp;
        pyramid.draw(shader, modeltp);
        modeltp = model1 * transforamtion(0, 1.5, 0, .14, .14, .14);
        modeltp *= goaroundtp;
        pyramid.draw(shader, modeltp);
        modeltp = model1 * transforamtion(0, 2, 0, .7, .7, .7);
        modeltp *= goaroundtp;
        pyramid.draw(shader, modeltp);
        modeltp = model1 * transforamtion(0, 2.3, 0, .3, .3, .3);
        modeltp *= goaroundtp;
        pyramid.draw(shader, modeltp);

    }


}


void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 22.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //gun_y += 0.003f;

        camera.ProcessKeyboard(FORWARD, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //gun_y -= 0.003f;
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // gun_x += 0.003f;
        camera.ProcessKeyboard(LEFT, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        // gun_x -= 0.003f;
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        if (!shoot)
        {
            shoot = true;
            bullet_x = gun_x;
            bullet_y = gun_y;
            slope_X = tan(right_angle * 3.1416 / 180.0);
            slope_Y = -1*tan(up_angle * 3.1416 / 180.0);
        }
            
    }


    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        up_angle += 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        up_angle -= 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        right_angle += 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        right_angle -= 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (!pause)
            pause = true;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        if (pause)
            pause = false;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(YAW_L, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        camera.ProcessKeyboard(YAW_R, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(PITCH_D, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(PITCH_U, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(ROLL_R, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(ROLL_L, deltaTime);
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (directionLighton)
        {
            directlight.turnOff();
            directionLighton = !directionLighton;
        }
        else
        {
            directlight.turnOn();
            directionLighton = !directionLighton;
        }
    }
    if (key == GLFW_KEY_V && action == GLFW_PRESS)                   //Ferris Wheel On/Off
    {
        isFerrisWheelOn ^= true;
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointlight5.turnOff();
            pointlight6.turnOff();
            pointlight7.turnOff();
            pointlight8.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointlight5.turnOn();
            pointlight6.turnOn();
            pointlight7.turnOn();
            pointlight8.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (spotlighton)
        {
            spotlight.turnOff();
            spotlighton = !spotlighton;
        }
        else
        {
            spotlight.turnOn();
            spotlighton = !spotlighton;
        }
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            directlight.turnAmbientOff();
            spotlight.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            directlight.turnAmbientOn();
            spotlight.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            directlight.turnDiffuseOff();
            spotlight.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            directlight.turnDiffuseOn();
            spotlight.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (specularToggle)
        {
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();
            directlight.turnSpecularOff();
            spotlight.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            directlight.turnSpecularOn();
            spotlight.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}