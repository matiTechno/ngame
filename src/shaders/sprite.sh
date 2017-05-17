R"(

#VERTEX

#version 430

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

#version 430

out vec4 color;

uniform bool is_texture;
uniform sampler2D sampl;

in vec4 v_color;
in vec2 v_tex_coord;

void main()
{
    if(is_texture)
        color = texture(sampl, v_tex_coord) * v_color;
    else
        color = v_color;
}

)"
