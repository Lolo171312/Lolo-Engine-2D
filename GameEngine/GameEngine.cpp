#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "STBImage/stb_image.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <cmath>
#include "Shader.h"
#include "Camera.h"
#include "LObject.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

float deltaTime = 0.0f;
float prevTime = 0.0f;

//Create vertex Shader
const char* vertexShaderSource = "#version 330 core\n" //Create shader source C-style string
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoords;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 projection;\n"
"uniform vec3 color;"
"\n"
"out vec3 Color;"
"out vec2 TexCoords;"
"\n"
"void main()\n"
"{\n"
//"   gl_Position = projection * model * vec4(aPos, 1.0f);\n"
"   gl_Position = projection * model * vec4(aPos, 1.0f);\n"
"   Color = color;"
"   TexCoords = aTexCoords;"
"}\0";

//Create fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;"
"\n"
"uniform sampler2D texture1;"
"\n"
"in vec2 TexCoords;"
"\n"
"void main()\n"
"{\n"
"   FragColor = texture(texture1, TexCoords);"
"}\0";

const char* simpleVertexShaderSource = "#version 330 core\n" //Create shader source C-style string
"layout (location = 0) in vec2 aPos;\n"
"layout (location = 1) in vec2 aTexCoords;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 projection;\n"
"\n"
"out vec2 TexCoords;"
"\n"
"void main()\n"
"{\n"
"   gl_Position = projection * model * vec4(aPos, -5.0f, 1.0f);\n"
"   TexCoords = aTexCoords;"
"}\0";

//Create fragment shader
const char* simpleFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;"
"\n"
"in vec2 TexCoords;"
"\n"
"uniform sampler2D texture1;"
"\n"
"void main()\n"
"{\n"
"   FragColor = texture(texture1, TexCoords);"
"}\0";

//Called every time the viewport´s size is set
void FrameBuffer_Size_Callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}

//This function will process every Input of the client
void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(void)
{
    //Initialize GLFW
    if (!glfwInit()) 
    {
        glfwTerminate();
        return -1;
    }

    //Tell GLFW we want to use OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Tell GLFW we want to use a version of OpenGL with less option we won´t need
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Lolo Engine 2D", NULL, NULL);
    if (window == nullptr) 
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    //Indicate OpenGL the size of the rendering window
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    //FrameBuffer_Size_Callback func will be called every time the viewport´s size is set
    glfwSetFramebufferSizeCallback(window, FrameBuffer_Size_Callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    Shader myShader(simpleVertexShaderSource, simpleFragmentShaderSource);
    myShader.UseShader();
    float horizontalCoord = (float)WINDOW_WIDTH;
    float verticalCoord = (float)WINDOW_HEIGHT;
    glm::mat4 projection = glm::ortho(0.0f, horizontalCoord, verticalCoord, 0.0f, 0.1f, 100.0f);
    myShader.SetMatrix4Uniform("projection", glm::value_ptr(projection));
    myShader.SetIntUniform("texture1", 0);

    LObject* myObject = new LObject("../Content/bee.png", &myShader);
    LObject* myOtherObject = new LObject("../Content/container2.png", &myShader);

    while (!glfwWindowShouldClose(window))
    {
        //Get Delta Time
        float currentTime = glfwGetTime();
        deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        //Input
        ProcessInput(window);
        myObject->SetObjectLocation(glm::vec2(470.0f, 300.0f));

        //Rendering
        glClearColor(0.2f, 0.8f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myOtherObject->Update();
        myObject->Update();

        //Check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}