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

uniform float time;
uniform float vel = 100;
uniform float len = 70;
uniform ivec4 scene_coords;
uniform float Pi = 3.14;

in vec2 v_tex_coord;

void main()
{
    float f = vel / len;
    vec2 center = scene_coords.xy + scene_coords.zw / 2;
    float r = length(center - gl_FragCoord.xy);
    float h = sin(2.0 * Pi / len * r - 2.0 * Pi * f * time) / 2.0 + 0.5;
    color = texture(sampl, v_tex_coord) * h;
}
