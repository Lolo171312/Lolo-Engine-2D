#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <cstdlib>
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
#include "CAudioSource.h"
#include "ObjectsManager.h"
#include "ColliderManager.h"
#include "Flyweight/TextureFactory.h"
#include "Sound.h"
#include "Breakout/Block.h"
#include "Breakout/Paddle.h"
#include "Breakout/Ball.h"
#include "Helpers.h"

const int WINDOW_WIDTH = 576;
const int WINDOW_HEIGHT = 650;

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
    srand(time(0));

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

    //Create GLFW window and make context current
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


    //Initialize TextRenderer
    if (InitSound() == -1)
    {
        std::cout << "Failed to initialize Sound" << std::endl;
        return -1;
    }

    //Initialize TextRenderer
    if (InitTextRenderer((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT) == -1)
    {
        std::cout << "Failed to initialize TextRenderer" << std::endl;
        return -1;
    }

    //Initialize Objects Manager
    ObjectsManager::Init();

#pragma region CreateShaderAndObjects
    //Create base shader for every object
    Shader myShader("Shaders/shader.vs.txt", "Shaders/shader.fs.txt");
    myShader.UseShader();
    float horizontalCoord = (float)WINDOW_WIDTH;
    float verticalCoord = (float)WINDOW_HEIGHT;
    glm::mat4 projection = glm::ortho(0.0f, horizontalCoord, verticalCoord, 0.0f, 0.1f, 100.0f);
    myShader.SetMatrix4Uniform("projection", glm::value_ptr(projection));
    myShader.SetIntUniform("texture1", 0);
    myShader.SetVec3Uniform("color", glm::vec3(1.0f, 1.0f, 1.0f));
    glUseProgram(0);

    //Create every block using two for loops
    int horizontalOffset = 64;
    int verticalOffset = 64;
    unsigned int horizontalBlocks = 8;
    unsigned int verticalBlocks = 6;
    for (unsigned int x = 0; x < horizontalBlocks; ++x)
    {
        for (unsigned int y = 0; y < verticalBlocks; ++y)
        {
            //Create block object with its tag
            Block* blockObj = new Block(&myShader, "Block");
            //Generate TextureRenderer Cmp with the brick texture and attach it to the object
            CTextureRenderer* blockTextureCmp = new CTextureRenderer("../Content/brick.png");
            blockObj->AttachComponent(blockTextureCmp);
            //Create ColliderCmp and attach it to the brick
            CColliderRect* blockColliderCmp = new CColliderRect(64.0f, 32.0f);
            blockObj->AttachComponent(blockColliderCmp);
            //Modify location
            blockObj->SetObjectLocation(glm::vec2(horizontalOffset + x * 64, verticalOffset + y * 32));
            //Define block´s color using the y value
            if(y < 2) blockTextureCmp->SetColor(glm::vec3(1.0f, 0.0f, 0.0f)); //Red
            else if(y < 4) blockTextureCmp->SetColor(glm::vec3(0.0f, 1.0f, 0.0f)); //Green
            else blockTextureCmp->SetColor(glm::vec3(0.0f, 0.0f, 1.0f)); //Blue
        }
    }

    //Create Paddle object
    Paddle* paddleObj = new Paddle(&myShader, "Paddle", window);
    CTextureRenderer* paddleTextureCmp = new CTextureRenderer("../Content/paddle.png");
    CColliderRect* paddleColliderCmp = new CColliderRect(128.0f, 32.0f);
    paddleObj->AttachComponent(paddleTextureCmp);
    paddleObj->AttachComponent(paddleColliderCmp);
    paddleObj->SetObjectLocation(glm::vec2((float)WINDOW_WIDTH / 2.0f, (float)WINDOW_HEIGHT - 64));

    //Create Ball object
    Ball* ballObj = new Ball(&myShader, "Ball", window);
    CTextureRenderer* ballTextureCmp = new CTextureRenderer("../Content/ball.png");
    CColliderCircle* ballColliderCmp = new CColliderCircle(8.0f);
    ballObj->AttachComponent(ballTextureCmp);
    ballObj->AttachComponent(ballColliderCmp);
    ballObj->SetObjectLocation(glm::vec2((float)WINDOW_WIDTH / 2.0f, (float)WINDOW_HEIGHT - 90));

#pragma endregion CreateShaderAndObjects

    while (!glfwWindowShouldClose(window))
    {
        //Get Delta Time
        float currentTime = glfwGetTime();
        deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        //Input
        ProcessInput(window);

        //Rendering
        glClearColor(0.741f, 0.741f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Update every Object in the game
        ObjectsManager::GetInstance()->Update(deltaTime);

        //Check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Terminate OpenAl related elements
    TerminateSound();
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