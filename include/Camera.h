#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Camera
{
public:
    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };
public:
    Camera() = delete;
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    // constructor with vectors
    Camera( glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = -90.0f,
            float pitch = 0.0f )
    {
        m_Position = position;
        m_WorldUp = up;
        m_Yaw = yaw;
        m_Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = m_MovementSpeed * deltaTime;
        if (direction == FORWARD)
        {
            m_Position += m_Front * velocity;
        }
        if (direction == BACKWARD)
        {
            m_Position -= m_Front * velocity;
        }
        if (direction == LEFT)
        {
            m_Position -= m_Right * velocity;
        }
        if (direction == RIGHT)
        {
            m_Position += m_Right * velocity;
        }
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;

        m_Yaw   += xoffset;
        m_Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (m_Pitch > 89.0f)
            {
                m_Pitch = 89.0f;
            }
            if (m_Pitch < -89.0f)
            {
                m_Pitch = -89.0f;
            }
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        m_Zoom -= (float)yoffset;
        if (m_Zoom < 1.0f)
        {
            m_Zoom = 1.0f;
        }
        if (m_Zoom > 45.0f)
        {
            m_Zoom = 45.0f;
        }
    }

    float GetZoom() const
    {
        return m_Zoom;
    };

private:;
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_Up    = glm::normalize(glm::cross(m_Right, m_Front));
    }
private:
    // camera Attributes
    glm::vec3 m_Position;
    glm::vec3 m_Up;
    glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;
    // camera options
    float m_MovementSpeed = 5.5f;
    float m_MouseSensitivity = 0.1f;
    float m_Zoom = 45.0f;
    // euler Angles
    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;
};