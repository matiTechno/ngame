#VERTEX

#version 430

layout(location = 0) in vec4 vertex;

out vec2 tex_coord;

void main()
{
	tex_coord = vertex.zw;
	gl_Position = vec4(vertex.xy, 0, 1);
}

#FRAGMENT

#version 430

in vec2 tex_coord;
uniform sampler2D sampl;

out vec4 color;

void main()
{
	color = texture(sampl, tex_coord);
}
