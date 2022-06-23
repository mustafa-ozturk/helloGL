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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

int main()
{
    Window window(800, 600, "helloGL");

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

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!window.ShouldClose())
    {
        window.ProcessInputs();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        VAO.Bind();

        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        // A bit farther away from us.
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.5f));

        // perspective with 45 fov
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        shader.SetUniformMat4f("model", model);
        shader.SetUniformMat4f("view", view);
        shader.SetUniformMat4f("projection", projection);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


        window.SwapBuffersAndPollEvents();
    }

    return 0;
}
