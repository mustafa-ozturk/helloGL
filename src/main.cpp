#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "Shader.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "Texture.h"

#include "stb_image/stb_image.h"

void printGLInfo();
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Hello OpenGL";

int main()
{
    // glfw initialization and window creation
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // glew initialization
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
        return -1;
    }
    printGLInfo();

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    // update gl viewport when glfw window resizes
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    VertexArrayObject VAO;
    VertexBufferObject VBO(vertices, sizeof(vertices));
    ElementBufferObject EBO(indices, sizeof(indices));

    // position attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coords attribute pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    VBO.UnBind();
    VAO.UnBind();

    Shader shader("res/shaders/BasicVertex.glsl", "res/shaders/BasicFragment.glsl");

    Texture texure0("res/textures/Trollface.png", GL_TEXTURE_2D, GL_RGBA, "texture0");
    texure0.Set2DTextureWrapping(GL_REPEAT, GL_REPEAT);
    texure0.Set2DTextureFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Texture texture1("res/textures/TrollDespair.png", GL_TEXTURE_2D, GL_RGB, "texture1");
    texture1.Set2DTextureWrapping(GL_REPEAT, GL_REPEAT);
    texture1.Set2DTextureFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    shader.UseShader();

    // setting the uniform sampler
    shader.setUniformInt(texure0.GetTextureName(), texure0.GetTextureID() - 1);
    shader.setUniformInt(texture1.GetTextureName(), texture1.GetTextureID() - 1);

    float mix = 0.0f;
    float increment = 0.005f;

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        texure0.Bind();
        texture1.Bind();

        VAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        shader.setUniformFloat("u_mix", mix);

        if (mix >= 1.0)
        {
            increment = -0.005f;
        }
        else if (mix <= 0.0)
        {
            increment = 0.005f;
        }
        std::cout << mix << std::endl;
        mix += increment;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void printGLInfo()
{
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "openGL version: " << glGetString(GL_VERSION) << std::endl;
    int maxVertexAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);
    std::cout << "Maximum vertex attributes supported: " << maxVertexAttributes << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}