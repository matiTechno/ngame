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

uniform vec2 gpos;
uniform bool is_active;
uniform float dt;

uniform float drag_coeff = 0.05;
uniform float gravity_coeff = 50000;
uniform float max_gravity = 2000;

void main()
{
    uint id = gl_GlobalInvocationID.x;

    vec2 gravity = vec2(0);

    if(is_active)
    {
        vec2 diff = gpos - poss[id];
        gravity = normalize(diff) * min(max_gravity, (1 / pow(length(diff), 2)) * gravity_coeff);
    }

    vec2 drag = clamp(-normalize(vels[id]) * pow(length(vels[id]), 2) * drag_coeff, -abs(vels[id] / dt), abs(vels[id]));

    vec2 force = gravity + drag;
    poss[id] += vels[id] * dt + 0.5 * force * pow(dt, 2);
    vels[id] += force * dt;
}
