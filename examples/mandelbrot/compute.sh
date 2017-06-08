#VERTEX
#version 330
layout(location = 0) in vec4 vertex;
void main()
{
    gl_Position = vec4(vertex.xy, 0, 1);
}
#FRAGMENT
#version 330

out vec4 color;

uniform vec2 start;
uniform vec2 range;
uniform ivec2 fb_size;

uniform int iterations;

void main()
{
    vec2 fcoord = gl_FragCoord.xy;
    vec2 ncoord = vec2(fcoord.x, -(fcoord.y - fb_size.y));
    vec2 pos = start + ncoord * (range / fb_size);
    vec2 z = vec2(0);
    int it = 0;
    while(pow(z.x, 2) + pow(z.y, 2) < 4 && it < iterations)
    {
        float xtemp = pow(z.x, 2) - pow(z.y, 2) + pos.x;
        z.y = 2 * z.x * z.y + pos.y;
        z.x = xtemp;
        ++it;
    }
    color = vec4(vec3(float(it) / iterations), 1);
}
