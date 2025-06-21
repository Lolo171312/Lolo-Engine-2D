#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
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

    Shader myShader("Shaders/shader.vs.txt", "Shaders/shader.fs.txt");
    myShader.UseShader();
    float horizontalCoord = (float)WINDOW_WIDTH;
    float verticalCoord = (float)WINDOW_HEIGHT;
    glm::mat4 projection = glm::ortho(0.0f, horizontalCoord, verticalCoord, 0.0f, 0.1f, 100.0f);
    myShader.SetMatrix4Uniform("projection", glm::value_ptr(projection));
    myShader.SetIntUniform("texture1", 0);
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

    TextRenderer myTextRenderer(45.0f, glm::vec2(horizontalCoord, verticalCoord));
    Font basisFont = myTextRenderer.LoadFont("../Content/Fonts/Dotrice.otf", 45.0f);
    Font monospacedFont = myTextRenderer.LoadFont("../Content/Fonts/Monospace.ttf", 45.0f);

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

        myTextRenderer.RenderText(basisFont, "Hola como estas?" + std::to_string(10), glm::vec2(50.0f, 50.0f), scaleVal);
        myTextRenderer.RenderText(monospacedFont, "Yo: Estoy bieen ;)", glm::vec2(120.0f, 450.0f), 1.0f, glm::vec3(0.0f, 0.5f, 0.5f));
        myTextRenderer.RenderText(basisFont, "Nums = 1234567890!!!", glm::vec2(120.0f, 200.0f), 1.25f, glm::vec3(1.0f, 0.0f, 0.0f));
        myTextRenderer.RenderText("Using default Font :o", glm::vec2(250.0f, 145.0f), 1.00f, glm::vec3(1.0f, 1.0f, 0.0f));

        //Check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}