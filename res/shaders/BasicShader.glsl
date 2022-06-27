#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // read multiplication from right to left (has something to do with opengl)
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

#shader fragment
#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture0;

void main()
{
    FragColor = texture(texture0, TexCoord);
}