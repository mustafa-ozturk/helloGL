#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "Shader.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "Texture.h"
#include "Window.h"

#include "stb_image/stb_image.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    srand (time(NULL));
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    Window window(SCR_WIDTH, SCR_HEIGHT, "helloGL", camera);
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;;

    const std::vector<float> vertices = {
            // back
            0.5f,  0.5f, -0.5f,     1.0f, 1.0f,   // top right 0
            0.5f, -0.5f, -0.5f,     1.0f, 0.0f,   // bottom right 1
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,   // bottom left 2
            -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    // top left 3

            // front
            0.5f,  0.5f, 0.5f,      1.0f, 1.0f,   // top right 4
            0.5f, -0.5f, 0.5f,      1.0f, 0.0f,   // bottom right 5
            -0.5f, -0.5f, 0.5f,     0.0f, 0.0f,   // bottom left 6
            -0.5f,  0.5f, 0.5f,     0.0f, 1.0f,    // top left 7

            // right side
            0.5f,  0.5f, 0.5f,      1.0f, 1.0f,   // top right front 8
            0.5f, -0.5f, 0.5f,      1.0f, 0.0f,   // bottom right front 9
            0.5f,  0.5f, -0.5f,     0.0f, 1.0f,   // top right back 10 (from the side: top-left)
            0.5f, -0.5f, -0.5f,     0.0f, 0.0f,   // bottom right back 11 (from the side: bottom-left)

            // left side
            -0.5f,  0.5f, 0.5f,     0.0f, 1.0f,    // top left front 12
            -0.5f, -0.5f, 0.5f,     0.0f, 0.0f,   // bottom left front 13
            -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    // top left back 14 (from the side: top-right)
            -0.5f, -0.5f, -0.5f,    1.0f, 0.0f,   // bottom left back 15 (from the side: bottom-right)

            // top
            0.5f,  0.5f, 0.5f,      1.0f, 0.0f,   // top right front 16 (from the top: bottom-right)
            0.5f,  0.5f, -0.5f,     1.0f, 1.0f,   // top right back 17 (from the top: top-right)
            -0.5f,  0.5f, 0.5f,     0.0f, 0.0f,    // top left front 18 (from the top: bottom-left)
            -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    // top left back 19 (from the top: top-left)

            // bottom
            0.5f, -0.5f, 0.5f,      1.0f, 0.0f,   // bottom right front 20
            0.5f, -0.5f, -0.5f,     1.0f, 1.0f,   // bottom right back 21
            -0.5f, -0.5f, 0.5f,     0.0f, 0.0f,   // bottom left front 22
            -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   // bottom left back 23
    };
    const std::vector<unsigned int> indices = {
            // back
            0, 1, 2, // first triangle
            0, 2, 3,  // second triangle

            // front
            4, 5, 6,
            4, 6, 7,

            // right side
            8, 9, 11,
            8, 10, 11,

            // left side
            12, 13, 15,
            12, 14, 15,

            // top
            16, 17, 18,
            18, 17, 19,

            // bottom
            20, 21, 22,
            22, 21, 23,
    };

    VertexArrayObject VAO;
    VertexBufferObject VBO(vertices);
    ElementBufferObject EBO(indices);

    // position attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coords attribute pointer
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    VBO.UnBind();
    VAO.UnBind();

    Shader shader("res/shaders/BasicShader.glsl");

    Texture texture0("res/textures/wooden.png");
    texture0.Bind();

    shader.UseShader();

    std::vector<glm::vec3> cubePositions = {};
    for (int i = 0; i < 250; i++)
    {
        int randomNum = rand() % 25;
        if (i % 3 == 0)
        {
            cubePositions.emplace_back( 0.0f + randomNum,  2.0f, 0.0f - i - 1 - randomNum);
            cubePositions.emplace_back( 0.0f + randomNum,  1.0f, 0.0f - i - 1 - randomNum);
            cubePositions.emplace_back( 0.0f + randomNum,  0.0f,  0.0f - i - 1 - randomNum);
            cubePositions.emplace_back( 0.0f + randomNum,  -1.0f, 0.0f - i - 1 - randomNum);

            cubePositions.emplace_back( 3.0f + randomNum,  2.0f, 0.0f - i - 1 - randomNum);
            cubePositions.emplace_back( 3.0f + randomNum,  1.0f, 0.0f - i - 1 - randomNum);
            cubePositions.emplace_back( 3.0f + randomNum,  0.0f,  0.0f - i - 1 - randomNum);
            cubePositions.emplace_back( 3.0f + randomNum,  -1.0f, 0.0f - i - 1 - randomNum);

            cubePositions.emplace_back( 1.0f + randomNum,  2.0f, 0.0f - i - 1 - randomNum);
            cubePositions.emplace_back( 2.0f + randomNum,  2.0f, 0.0f - i - 1 - randomNum);
        }
        else if ( i % 2 == 0)
        {
            cubePositions.emplace_back( 0.0f - randomNum,  2.0f, 0.0f - i - 1 + randomNum);
            cubePositions.emplace_back( 0.0f - randomNum,  1.0f, 0.0f - i - 1 + randomNum);
            cubePositions.emplace_back( 0.0f - randomNum,  0.0f,  0.0f - i - 1 + randomNum);
            cubePositions.emplace_back( 0.0f - randomNum,  -1.0f, 0.0f - i - 1 + randomNum);

            cubePositions.emplace_back( 3.0f - randomNum,  2.0f, 0.0f - i - 1 + randomNum);
            cubePositions.emplace_back( 3.0f - randomNum,  1.0f, 0.0f - i - 1 + randomNum);
            cubePositions.emplace_back( 3.0f - randomNum,  0.0f,  0.0f - i - 1 + randomNum);
            cubePositions.emplace_back( 3.0f - randomNum,  -1.0f, 0.0f - i - 1 + randomNum);

            cubePositions.emplace_back( 1.0f - randomNum,  2.0f, 0.0f - i - 1 + randomNum);
            cubePositions.emplace_back( 2.0f - randomNum,  2.0f, 0.0f - i - 1 + randomNum);
        }
    }

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!window.ShouldClose())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        window.ProcessInputs(deltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        VAO.Bind();

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.GetZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        shader.SetUniformMat4f("projection", projection);

        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        shader.SetUniformMat4f("view", view);

        for (int i = 0; i < cubePositions.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            shader.SetUniformMat4f("model", model);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        }


        window.SwapBuffersAndPollEvents();
    }

    return 0;
}
