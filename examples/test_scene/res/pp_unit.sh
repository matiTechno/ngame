#VERTEX

#version 430

layout(location = 0) in vec4 vertex;

out vec2 v_tex_coord;

void main()
{
    gl_Position = vec4(vertex.xy, 0, 1);
    v_tex_coord = vertex.zw;
}

#FRAGMENT

#version 430

out vec4 color;
uniform sampler2D sampl;

in vec2 v_tex_coord;

uniform float time;

void main()
{
    color = texture(sampl, v_tex_coord) * (sin(time) + 1) / 2
    * vec4(1, 0.5, 0.5, 0.5);
}
