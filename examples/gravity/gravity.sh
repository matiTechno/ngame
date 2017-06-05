#COMPUTE

#version 430

layout(local_size_x = 1000, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer buff1
{
    vec2 poss[];
};

layout(std430, binding = 1) buffer buff2
{
    vec2 vels[];
};

uniform vec2 g_pos;
uniform bool g_active;
uniform float dt;

uniform float drag = 1;
uniform float gravity = 10000;

void main()
{
    uint id = gl_GlobalInvocationID.x;

    vec2 f_drag = -normalize(vels[id]) * pow(length(vels[id]), 2) * drag;
    poss[id] += vels[id] * dt + 0.5 * f_drag * pow(dt, 2);
    vels[id] += f_drag * dt;

    if(g_active)
    {
        vec2 diff = g_pos - poss[id];
        vec2 f_gravity = normalize(diff) * (1 / pow(length(diff), 2)) * gravity;
        poss[id] += 0.5 * f_gravity * pow(dt, 2);
        vels[id] += f_gravity * dt;
    }
}
