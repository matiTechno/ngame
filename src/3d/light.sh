R"(

#VERTEX
#version 330

layout(location = 0) in vec3 pos;
uniform mat4 proj;
uniform mat4 view;
uniform float scale;
uniform vec3 translate;

void main()
{
    gl_Position = proj * view * vec4(pos * scale + translate, 1);
}

#FRAGMENT
#version 330
out vec4 color;
uniform vec3 u_color;

void main()
{
    color = vec4(u_color, 1);
}

)"
