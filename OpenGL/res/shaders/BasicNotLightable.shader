#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 aNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{ 
    gl_Position = projection * view * model * position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

uniform vec4 objectColor;

void main()
{
    FragColor = objectColor;

};