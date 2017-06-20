R"(
#VERTEX
#version 330

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;
layout(location = 3) in mat4 modelMatrix;

out vec2 vTexCoord;
out vec4 vColor;

uniform mat4 projection;

void main()
{
        gl_Position = projection * modelMatrix * vec4(pos, 0, 1);
        vTexCoord = texCoord;
        vColor = color;
}
#FRAGMENT
#version 330

out vec4 color;
uniform sampler2D sampl;
in vec2 vTexCoord;
in vec4 vColor;

uniform int renderType;

void main()
{
        if(renderType == 0)
                color = vColor;

        else if(renderType == 1)
                color = texture(sampl, vTexCoord) * vColor;

        else if(renderType == 2)
                color = vec4(1, 1, 1, texture(sampl, vTexCoord).r) * vColor;
}
)"
