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
#include "AL/al.h"
#include "AL/alc.h"
#include "AudioBuffer.h"
#include "CAudioSource.h"
#include "ObjectsManager.h"
#include "ColliderManager.h"
#include "Flyweight/TextureFactory.h"

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

    //Initialize OpenAl and open default Device
    ALCdevice* device = alcOpenDevice(NULL);
    if (!device)
    {
        return -1;
    }

    //Create OpenAl Context
    ALCcontext* context = alcCreateContext(device, NULL);
    if (!context)
    {
        alcCloseDevice(device);
        return -1;
    }
    //Make the created Context the current Context
    alcMakeContextCurrent(context);

    //Modify some general values of the Listener
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_ORIENTATION, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);

    //Initialize Objects Manager
    ObjectsManager::Init();

    //Initialize Texture Factory (FlyweightPatron)
    TextureFactory::Init();

#pragma region CreateShaderAndObjects
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
    myObject->AttachComponent(TextureRenderer);

    LObject* myObject2 = new LObject(&myShader);
    myObject2->SetObjectLocation(glm::vec2(180.0f, 300.0f));
    CTextureRenderer* TextureRenderer2 = new CTextureRenderer("../Content/bee.png");
    myObject2->AttachComponent(TextureRenderer2);

    LObject* myObject3 = new LObject(&myShader);
    myObject3->SetObjectLocation(glm::vec2(380.0f, 300.0f));
    CTextureRenderer* TextureRenderer3 = new CTextureRenderer("../Content/bee.png");
    myObject3->AttachComponent(TextureRenderer3);

    LObject* myObject4 = new LObject(&myShader);
    myObject4->SetObjectLocation(glm::vec2(280.0f, 500.0f));
    CTextureRenderer* TextureRenderer4 = new CTextureRenderer("../Content/bee.png");
    myObject4->AttachComponent(TextureRenderer4);

    LObject* myObject5 = new LObject(&myShader);
    myObject5->SetObjectLocation(glm::vec2(180.0f, 500.0f));
    CTextureRenderer* TextureRenderer5 = new CTextureRenderer("../Content/bee.png");
    myObject5->AttachComponent(TextureRenderer5);

    LObject* myObject6 = new LObject(&myShader);
    myObject6->SetObjectLocation(glm::vec2(380.0f, 500.0f));
    CTextureRenderer* TextureRenderer6 = new CTextureRenderer("../Content/bee.png");
    myObject6->AttachComponent(TextureRenderer6);
#pragma endregion CreateShaderAndObjects

    //Initialize TextRenderer
    if (InitTextRenderer(horizontalCoord, verticalCoord) == -1)
    {
        std::cout << "Failed to initialize TextRenderer" << std::endl;
        return -1;
    }

    //Load some fonts
    Font basisFont = LoadFont("../Content/Fonts/Dotrice.otf", 45.0f);
    Font monospacedFont = LoadFont("../Content/Fonts/Monospace.ttf", 55.0f);

    float speed = 200.0f;

    //Load some clips
    AudioBuffer* rockBuffer = AudioBuffer::Load("../Content/Sounds/RockIntro.wav");
    AudioBuffer* loopBuffer = AudioBuffer::Load("../Content/Sounds/Loop.wav");

    bool destroyObject = false;

    while (!glfwWindowShouldClose(window))
    {
        //Get Delta Time
        float currentTime = glfwGetTime();
        deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        //Input
        ProcessInput(window);

        //Rendering
        glClearColor(0.2f, 0.8f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ObjectsManager::GetInstance()->Update(deltaTime);

        float scaleVal = sinf(glfwGetTime());
        scaleVal = glm::clamp(scaleVal, 0.1f, 1.0f);

        RenderText(basisFont, "H\nola como\nestas?" + std::to_string(10), glm::vec2(50.0f, 50.0f), scaleVal);
        RenderText(monospacedFont, "Yo: Estoy bieen ;)", glm::vec2(120.0f, 450.0f), 1.0f, glm::vec3(0.0f, 0.5f, 0.5f));
        RenderText(basisFont, "Nums = 1234567890!!!", glm::vec2(120.0f, 200.0f), 1.25f, glm::vec3(1.0f, 0.0f, 0.0f));
        RenderText("Using default Font :o", glm::vec2(250.0f, 145.0f), 1.00f, glm::vec3(1.0f, 1.0f, 0.0f));

        float scale = sinf(glfwGetTime());
        myObject->SetObjectScale(glm::vec2(scale, scale));
        myObject5->SetObjectAngle(scale * 20.0f);

        //Check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Delete clips
    delete rockBuffer;
    delete loopBuffer;

    //Terminate OpenAl related elements
    alcDestroyContext(context);
    alcCloseDevice(device);
    //Terminate ObjectsManager
    ObjectsManager::GetInstance()->DestroyManager();
    //Terminate Texture Factory
    TextureFactory::GetInstance()->DestroyFactory();
    //Terminate Collider Manager
    ColliderManager::Destroy();
    //Terminate GLFW related elements
    glfwTerminate();
    //Terminate Text Renderer
    DestroyTextRenderer();

    return 0;
}