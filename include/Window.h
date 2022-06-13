#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
public:
    Window() = delete;
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(const int screenWidth, const int screenHeight, const std::string& title)
        : m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight), m_WindowTitle(title)
    {
        // glfw initialization and window creation
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(m_ScreenWidth, m_ScreenHeight, m_WindowTitle.c_str(), nullptr, nullptr);
        if (!m_Window)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(1);

        // glew initialization
        if (glewInit() != GLEW_OK)
        {
            std::cout << "Error!" << std::endl;
        }

        glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);

        // update gl viewport when glfw window resizes
        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height){
            glViewport(0, 0, width, height);
        });

        PrintGLInfo();
    }
    ~Window()
    {
        glfwTerminate();
    }
    void ProcessInputs()
    {
        if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(m_Window, true);
        }
    }
    void SwappBuffersAndPollEvents()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
    bool ShouldClose()
    {
        return glfwWindowShouldClose(m_Window);
    }
private:
    int m_ScreenWidth = 0;
    int m_ScreenHeight = 0;
    std::string m_WindowTitle;
    GLFWwindow* m_Window;

    void PrintGLInfo()
    {
        std::cout << "-------------------------------------------------" << std::endl;
        std::cout << "openGL version: " << glGetString(GL_VERSION) << std::endl;
        int maxVertexAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);
        std::cout << "Maximum vertex attributes supported: " << maxVertexAttributes << std::endl;
        std::cout << "-------------------------------------------------" << std::endl;
    }
};