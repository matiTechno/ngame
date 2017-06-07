#VERTEX

#version 330

layout(location = 0) in vec2 pos;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(pos, 0, 1);
}

#FRAGMENT

#version 330

out vec4 color;

void main()
{
    color = vec4(1, 0.5, 0, 0.3);
}
