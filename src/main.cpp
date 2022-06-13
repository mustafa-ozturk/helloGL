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
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };
    const std::vector<unsigned int> indices = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    VertexArrayObject VAO;
    VertexBufferObject VBO(vertices);
    ElementBufferObject EBO(indices);

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

    Texture texture0("res/textures/Trollface.png", GL_TEXTURE_2D, GL_RGBA, "texture0");
    texture0.Set2DTextureWrapping(GL_REPEAT, GL_REPEAT);
    texture0.Set2DTextureFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Texture texture1("res/textures/TrollDespair.png", GL_TEXTURE_2D, GL_RGB, "texture1");
    texture1.Set2DTextureWrapping(GL_REPEAT, GL_REPEAT);
    texture1.Set2DTextureFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    shader.UseShader();

    // setting the uniform sampler
    shader.setUniformInt(texture0.GetTextureName(), texture0.GetTextureID() - 1);
    shader.setUniformInt(texture1.GetTextureName(), texture1.GetTextureID() - 1);

    float mix = 0.0f;
    float increment = 0.005f;

    // query transform uniform location
    unsigned int transformLoc = glGetUniformLocation(shader.GetShaderProgram(), "transform");

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!window.ShouldClose())
    {
        window.ProcessInputs();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        texture0.Bind();
        texture1.Bind();

        VAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        shader.setUniformFloat("u_mix", mix);

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // set the transform uniform value
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        if (mix >= 1.0)
        {
            increment = -0.005f;
        }
        else if (mix <= 0.0)
        {
            increment = 0.005f;
        }
        mix += increment;

        window.SwappBuffersAndPollEvents();
    }

    return 0;
}
