R"(
#VERTEX
#version 330

layout(location = 0) in vec3 pos;

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

void main()
{
    color = vec4(u_color, 1);
}
)"
