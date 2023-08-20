/* Hatcher Blair
*  SNHU CS-330 Comp Graphics and Visualization
*  Final Project
*  7/2023 V 1.0
*  This Program renders my keyboard, cable, and desk mat using OpenGL
*  
*  TODO: Implement Rendering of text on keys
*/

#include <iostream>         // cout, cerr

// Header file inclusions
#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "Mesh.h"
#include "Torus.h"
#include "Cylinder.h"
#include "Keyboard.h"

// Window height, width, and title
const char* WINDOW_TITLE = "Keyboard Render";
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

/* Mouse Mat Vertices and Indices*/
// Vertices, Dimensions (90 x 40) cm.
Vertex mouseMatVertices[] = {
    // Positions                            // Colors (r,g,b,a)                // Normals                    // Texture coords           
    Vertex{glm::vec3(-45.0f, 0.0f, 20.0f),  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec2(1.0f, 1.0f)},    // Left Back   0
    Vertex{glm::vec3(45.0f, 0.0f, 20.0f),   glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec2(0.0f, 1.0f) },  // Right Back  1
    Vertex{glm::vec3(45.0f, 0.0f, -20.0f),  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec2(0.0f, 0.0f) },   // Right Front 2
    Vertex{glm::vec3(-45.0f, 0.0f, -20.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec2(1.0f, 0.0f) }    // Left Front  3
};

GLuint mouseMatIndices[] = {
    0, 1, 2,
    2, 3, 0
};

/* Function Declarations, Initialization and description under main */
bool UInitialize(GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
std::vector< std::vector <Vertex> > CreateKeyCapRow(Vertex* keyCapVertices, int numKeys, int numVertices, GLfloat* keyCapScale);
void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
Vertex RotateKeyCap(Vertex vertex);

// Create a camera object
Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(0.0f, 10.0f, 15.0f));

int main()
{
    // Initialize the window
    GLFWwindow* window;
    if (!UInitialize(&window))
        return EXIT_FAILURE;

    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(119 / 255.0f, 136 / 255.0f, 153 / 255.0f, 1.0f);

    // Generate the shader objects
    Shader shaderProgram("default.vert", "default.frag");

    // Disable the cursor by default
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Mesh for the keycaps
    std::vector <Mesh> keyCaps = CreateKeyCaps();

    // Mesh for the base of the keyboard
    Mesh keyboardBase = CreateKeyboardBase();

    // Create a vector containing the vertices for the mouse mat
    std::vector <Vertex> mouseMatVerts(mouseMatVertices, mouseMatVertices + sizeof(mouseMatVertices) / sizeof(Vertex));

    // Create a vector containing the indices for the mouse mat
    std::vector <GLuint> mouseMatInd(mouseMatIndices, mouseMatIndices + sizeof(mouseMatIndices) / sizeof(GLuint));

    // Creat a vector containing the texture for the mouse mat
    Texture deskMatTextures[] = {
        Texture("DeskMatTexture.png", "tex", 0, GL_RGBA, GL_UNSIGNED_BYTE),
    };
    std::vector <Texture> texMat(deskMatTextures, deskMatTextures + sizeof(deskMatTextures) / sizeof(Texture));

    // Create a mesh for the mouse mat
    Mesh mouseMat(mouseMatVerts, mouseMatInd, texMat);

    // Create a vecter containing the texture for the tori
    Texture cableTextures[] = {
        Texture("WhiteMeshBlueBack.png", "tex", 0, GL_RGBA, GL_UNSIGNED_BYTE),
    };
    std::vector <Texture> texCable(cableTextures, cableTextures + sizeof(cableTextures) / sizeof(Texture));

    // Creates 25 Tori to be used for the keyboard cable, and offsets each one by 2*r (inner radius)
    std::vector <Mesh> keyboardCableToriMesh;
    for (int i = 0; i < 25; i++) {
        std::vector <Vertex> torusVertices = createTorus(smallDiameter / 2, largeDiameter / 2, 30, 30, false, false);
        rotateTorus(torusVertices);
        translateTorus(torusVertices, i);
        Mesh torusMesh(torusVertices, texCable);
        keyboardCableToriMesh.push_back(torusMesh);
    }

    // Make the first cylinder, there are 3 meshes (sides, top, bottom)
    std::vector < std::vector <Vertex> > firstCylinderVertices = createCylinder(5.0, 10);
    rotateCylinder(firstCylinderVertices);
    translateCylinder(firstCylinderVertices, 1);
    Mesh firstCylinderSidesMesh(firstCylinderVertices[0], texCable);
    Mesh firstCylinderTopMesh(firstCylinderVertices[1], texCable);
    Mesh firstCylinderBottomMesh(firstCylinderVertices[2], texCable);

    // Make the second cylinder
    std::vector < std::vector <Vertex> > secondCylinderVertices = createCylinder(10.0, 10);
    rotateCylinder(secondCylinderVertices);
    translateCylinder(secondCylinderVertices, 2);
    Mesh secondCylinderSidesMesh(secondCylinderVertices[0], texCable);
    Mesh secondCylinderTopMesh(secondCylinderVertices[1], texCable);
    Mesh secondCylinderBottomMesh(secondCylinderVertices[2], texCable);

    // Set the position of the object
    glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 rotation = glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

    glm::mat4 model = translation * rotation * scale;

    // Create the light shader
    Shader lightShader("light.vert", "light.frag");

    // Create the mesh for the light
    Mesh lightMesh = CreateKeyboardLight();

    // Set the color, position and model for the lights
    glm::vec4 lightColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f, 0.25f, 0.0f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);
    glm::vec4 directLightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Pass the light color and model to the GPU (light shader)
    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

    // Pass the object and light position + color to the GPU (object shader)
    shaderProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "pointLightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "directLightColor"), directLightColor.x, directLightColor.y, directLightColor.z, directLightColor.w);

    // Enable Depth
    glEnable(GL_DEPTH_TEST);

    // Main Render Loop
    while (!glfwWindowShouldClose(window)) {

        // Handle Input
        camera.Inputs(window);

        // Set background color and clear back and depth buffer
        glClearColor(119 / 255.0f, 136 / 255.0f, 153 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Pass the projection information to the shaderProgram
        camera.UpdateMatrix(90.0f, 0.1f, 100.0f);

        // Draw all the mesh for the keyCaps
        for (int i = 0; i < keyCaps.size(); i++) {
            keyCaps[i].DrawCube(shaderProgram, camera);
        }

        // Draw the mesh for the keyboardBase
        keyboardBase.DrawCube(shaderProgram, camera);

        // Draw the mesh for the Mouse Mat
        mouseMat.DrawCube(shaderProgram, camera);

        // Draw the array of tori
        for (int i = 0; i < keyboardCableToriMesh.size(); i++) {
            keyboardCableToriMesh[i].DrawVertices(shaderProgram, camera);
        }

        // Draw the first cylinder
        firstCylinderSidesMesh.DrawStrips(shaderProgram, camera);
        firstCylinderTopMesh.DrawFan(shaderProgram, camera);
        firstCylinderBottomMesh.DrawFan(shaderProgram, camera);

        // Draw the second cylinder
        secondCylinderSidesMesh.DrawStrips(shaderProgram, camera);
        secondCylinderTopMesh.DrawFan(shaderProgram, camera);
        secondCylinderBottomMesh.DrawFan(shaderProgram, camera);

        // Draw the light
        lightMesh.DrawCube(lightShader, camera);
        
        // Swap buffers
        glfwSwapBuffers(window);

        // Poll for events
        glfwPollEvents();
    }

    // Delete all objects created
    shaderProgram.Delete();
    lightShader.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


// Initialize GLFW, GLEW, and create a window
bool UInitialize(GLFWwindow** window)
{
    // GLFW: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    // ---------------------
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetScrollCallback(*window, MouseScrollCallback);

    // GLEW: initialize
    // ----------------
    // Note: if using GLEW version 1.13 or earlier
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }
    return true;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// glfw: callback function for mouse scroll wheel
void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    float oldSpeed = camera.speed;
    if ((oldSpeed += 0.1f * yOffset) < 0) {
        camera.speed = 0.1f;
    }
    else {
        camera.speed += 0.1f * yOffset;
    }
}