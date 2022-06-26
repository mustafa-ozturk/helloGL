#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Camera.h"

class Window
{
public:
    Window() = delete;
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(const int screenWidth, const int screenHeight, const std::string& title, Camera& camera)
        : m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight), m_WindowTitle(title),
          m_LastX(screenWidth / 2.0f), m_LastY(screenHeight / 2.0f), m_Camera(camera)
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
        glfwSetWindowUserPointer(m_Window, static_cast<void*>(this));
        glfwSwapInterval(1);
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xposIn, double yposIn){
            auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));

            float xpos = static_cast<float>(xposIn);
            float ypos = static_cast<float>(yposIn);

            if (self->m_FirstMouse)
            {
                self->m_LastX = xpos;
                self->m_LastY = ypos;
                self->m_FirstMouse = false;
            }

            float xoffset = xpos - self->m_LastX;
            float yoffset = self->m_LastY - ypos; // reversed since y-coordinates go from bottom to top

            self->m_LastX = xpos;
            self->m_LastY = ypos;

            self->m_Camera.ProcessMouseMovement(xoffset, yoffset);
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset){
            auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
            self->m_Camera.ProcessMouseScroll(static_cast<float>(yoffset));
        });

        PrintGLInfo();

        glEnable(GL_DEPTH_TEST);
    }
    ~Window()
    {
        glfwSetWindowUserPointer(m_Window, nullptr);
        glfwTerminate();
    }
    void ProcessInputs(float deltaTime)
    {
        if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(m_Window, true);
        }
        if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
        {
            m_Camera.ProcessKeyboard(m_Camera.FORWARD, deltaTime);
        }
        if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
        {
            m_Camera.ProcessKeyboard(m_Camera.BACKWARD, deltaTime);
        }
        if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
        {
            m_Camera.ProcessKeyboard(m_Camera.LEFT, deltaTime);
        }
        if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
        {
            m_Camera.ProcessKeyboard(m_Camera.RIGHT, deltaTime);
        }
    }
    void SwapBuffersAndPollEvents()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
    bool ShouldClose()
    {
        return glfwWindowShouldClose(m_Window);
    }
    GLFWwindow* GetWindow() const
    {
        return m_Window;
    }
private:
    int m_ScreenWidth = 0;
    int m_ScreenHeight = 0;
    std::string m_WindowTitle;
    GLFWwindow* m_Window;
    bool m_FirstMouse = true;
    float m_LastX = 0;
    float m_LastY = 0;
    Camera& m_Camera;

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