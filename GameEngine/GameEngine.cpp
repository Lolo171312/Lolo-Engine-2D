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
#include "CTextureRenderer.h"
#include "CColliderCircle.h"
#include "CColliderRect.h"
#include "TextRenderer.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

float deltaTime = 0.0f;
float prevTime = 0.0f;

//Create vertex Shader
const char* vertexShaderSource = "#version 330 core\n" //Create shader source C-style string
"layout (location = 0) in vec2 aPos;\n"
"layout (location = 1) in vec2 aTextCoords;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 projection;\n"
"\n"
"out vec2 TextCoords;"
"\n"
"void main()\n"
"{\n"
"   gl_Position = projection * model * vec4(aPos, -5.0f, 1.0f);\n"
"   TextCoords = aTextCoords;"
"}\0";

//Create fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;"
"\n"
"in vec2 TextCoords;"
"\n"
"uniform sampler2D text;"
"uniform vec3 color;"
"\n"
"void main()\n"
"{\n"
"   vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(text, TextCoords).r);"
"   FragColor = vec4(color, 1.0f) * sampled;"
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
    glUseProgram(0);

    Shader textShader(vertexShaderSource, fragmentShaderSource);
    textShader.UseShader();
    textShader.SetMatrix4Uniform("projection", glm::value_ptr(projection));
    glUseProgram(0);

    LObject* myObject = new LObject(&myShader);
    myObject->SetObjectLocation(glm::vec2(280.0f, 300.0f));
    CTextureRenderer* TextureRenderer = new CTextureRenderer("../Content/bee.png");
    CColliderCircle* Collider = new CColliderCircle(46.0f);
    myObject->AttachComponent(TextureRenderer);
    myObject->AttachComponent(Collider);

    LObject* myOtherObject = new LObject(&myShader);
    myOtherObject->SetObjectLocation(glm::vec2(150.0f, 300.0f));
    CTextureRenderer* OtherTextureRenderer = new CTextureRenderer("../Content/box.png");
    CColliderRect* OtherCollider = new CColliderRect(100.0f, 100.0f);
    myOtherObject->AttachComponent(OtherTextureRenderer);
    myOtherObject->AttachComponent(OtherCollider);

    LObject* ballObject = new LObject(&myShader);
    ballObject->SetObjectLocation(glm::vec2(500.0f, 300.0f));
    CTextureRenderer* BallTextureRenderer = new CTextureRenderer("../Content/ball.png");
    CColliderCircle* BallCollider = new CColliderCircle(50.0f);
    ballObject->AttachComponent(BallTextureRenderer);
    ballObject->AttachComponent(BallCollider);

    TextRenderer myTextRenderer(45.0f, &textShader);

    float speed = 200.0f;

    while (!glfwWindowShouldClose(window))
    {
        //Get Delta Time
        float currentTime = glfwGetTime();
        deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        //Input
        ProcessInput(window);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) 
        {
            myObject->AddObjectLocation(glm::vec2(1.0f, 0.0f) * speed * deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            myObject->AddObjectLocation(glm::vec2(-1.0f, 0.0f) * speed * deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            myObject->AddObjectLocation(glm::vec2(0.0f, -1.0f) * speed * deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            myObject->AddObjectLocation(glm::vec2(0.0f, 1.0f) * speed * deltaTime);
        }

        //Rendering
        glClearColor(0.2f, 0.8f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myOtherObject->Update(deltaTime);
        myObject->Update(deltaTime);
        ballObject->Update(deltaTime);

        float scaleVal = sinf(glfwGetTime());
        scaleVal = glm::clamp(scaleVal, 0.1f, 1.0f);

        myTextRenderer.RenderText("Hola cómo estás?", glm::vec2(50.0f, 50.0f), scaleVal);
        myTextRenderer.RenderText("Yo: Estoy bieen ;)", glm::vec2(120.0f, 450.0f), 0.5f, glm::vec3(0.0f, 0.5f, 0.5f));
        myTextRenderer.RenderText("Nums = 1234567890!!!", glm::vec2(120.0f, 200.0f), 1.25f, glm::vec3(1.0f, 0.0f, 0.0f));

        //Check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}