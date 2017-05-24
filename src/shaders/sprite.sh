R"(

#VERTEX

#version 330

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec4 color;
layout(location = 2) in vec4 tex_coords;
layout(location = 3) in mat4 model;

uniform mat4 projection;

out vec4 v_color;
out vec2 v_tex_coord;

void main()
{
    gl_Position = projection * model * vec4(vertex.xy, 0, 1);
    v_color = color;
    v_tex_coord = vertex.zw * tex_coords.zw + tex_coords.xy;
}

#FRAGMENT

#version 330

out vec4 color;

uniform int type;
uniform sampler2D sampl;

in vec4 v_color;
in vec2 v_tex_coord;

void main()
{
    if(type == 0)
        color = v_color;

    else if(type == 1)
        color = texture(sampl, v_tex_coord) * v_color;

    else if(type == 2)
        color = vec4(1, 1, 1, texture(sampl, v_tex_coord).r) * v_color;
}

)"
