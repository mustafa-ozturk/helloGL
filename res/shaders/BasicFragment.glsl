#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float u_mix;

void main()
{
    // 0.2 == return 80% of the first input color and 20% of second input color
    FragColor =  mix(texture(texture0, TexCoord), texture(texture1, vec2(TexCoord.s, TexCoord.t)), u_mix);
}