R"(

#VERTEX
#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(pos, 1);
}

#FRAGMENT
#version 330
out vec4 color;
uniform vec3 u_color;
uniform float shininess;

struct Light
{
    vec3 position;
    vec3 color;
};

uniform Light lights[20];
uniform int num_lights = 0;

void main()
{
    color = vec4(u_color, 1);
}

)"
