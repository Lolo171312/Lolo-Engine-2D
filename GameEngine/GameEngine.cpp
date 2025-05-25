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
"in vec3 Color;"
"in vec2 TexCoords;"
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

unsigned int LoadTexture(const char* textureLocation) 
{
    unsigned int textureId;
    //stbi_set_flip_vertically_on_load(true);
    glGenTextures(1, &textureId);
    int textureX, textureY, nrComponents;
    stbi_uc* texData = stbi_load(textureLocation, &textureX, &textureY, &nrComponents, 0);
    if (texData)
    {
        GLenum format = GL_RED;
        if (nrComponents == 1)
        {
            format = GL_RED;
        }
        else if (nrComponents == 3)
        {
            format = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureX, textureY, 0, format, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(texData);
    }
    else
    {
        std::cout << "Texture failed to load" << std::endl;
        stbi_image_free(texData);
    }

    return textureId;
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

    //Create VBO and VBA
    float vertexData[]
    {
        80.0f, 92.0f, -5.0f, 1.0f, 1.0f,
        80.0f, 0.0f, -5.0f, 1.0f, 0.0f,
        0.0f, 0.0f, -5.0f, 0.0f, 0.0f,
        0.0f, 92.0f, -5.0f, 0.0f, 1.0f
    };

    unsigned int indexData[]
    {
        0, 1, 2,
        3, 0, 2
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), &vertexData, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), &indexData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    unsigned int texture = LoadTexture("../Content/bee.png");
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, texture);

    Camera myCamera;

    Shader myShader(vertexShaderSource, fragmentShaderSource);
    myShader.UseShader();
    
    myShader.SetMatrix4Uniform("model", glm::value_ptr(glm::mat4(1.0f)));
    float horizontalCoord = (float)WINDOW_WIDTH;
    float verticalCoord = (float)WINDOW_HEIGHT;
    glm::mat4 projection = glm::ortho(0.0f, horizontalCoord, verticalCoord, 0.0f, 0.1f, 100.0f);
    myShader.SetMatrix4Uniform("projection", glm::value_ptr(projection));
    myShader.SetVec3Uniform("color", glm::vec3(0.0f, 0.2f, 1.0f));
    myShader.SetIntUniform("texture1", 0);

    glEnable(GL_DEPTH_TEST);

    float speed = 120.0f;
    glm::vec3 position(0.0f);

    while (!glfwWindowShouldClose(window))
    {
        //Get Delta Time
        float currentTime = glfwGetTime();
        deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        //Input
        ProcessInput(window);

        //double mouseX, mouseY;
        //glfwGetCursorPos(window, &mouseX, &mouseY);
        //std::cout << "MousePos -> X: " << mouseX << " | Y: " << mouseY << std::endl;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
        {
            position.x += speed * deltaTime;
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, position);
            myShader.SetMatrix4Uniform("model", glm::value_ptr(modelMatrix));
            std::cout << "Position -> X: " << position.x << " | Y: " << position.y << " | Z: " << position.z << std::endl;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            position.x -= speed * deltaTime;
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, position);
            myShader.SetMatrix4Uniform("model", glm::value_ptr(modelMatrix));
        }

        //Rendering
        glClearColor(0.2f, 0.8f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //Check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}