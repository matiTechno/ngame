R"(

#VERTEX
#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 v_normal;
// doing it in view space would be better I guess
// but for now it doesn't matter
out vec3 wpos;

void main()
{
    gl_Position = proj * view * model * vec4(pos, 1);
    // not efficient for a long run
    v_normal = mat3(transpose(inverse(model))) * normal;
    wpos = vec3(model * vec4(pos, 1));
}

#FRAGMENT
#version 330
out vec4 color;
uniform vec3 cam_pos;

struct Light
{
    vec3 pos;
    vec3 color;
};

uniform Light lights[20];
uniform int num_lights = 0;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material mat;

in vec3 v_normal;
in vec3 wpos;

void main()
{
    vec3 col3 = vec3(0);
    for(int i = 0; i < num_lights; ++i)
        {
            vec3 nnorm = normalize(v_normal);
            vec3 frag_to_light = normalize(lights[i].pos - wpos);
            vec3 diffuse = max(dot(nnorm, frag_to_light), 0) * mat.diffuse;

            vec3 frag_to_cam = normalize(cam_pos - wpos);
            vec3 halfway = normalize(frag_to_light + frag_to_cam);
            vec3 specular = pow(max(dot(nnorm, halfway), 0), mat.shininess) * mat.specular;

            col3 += (mat.ambient + diffuse + specular) * lights[i].color;
        }
    color = vec4(col3, 1);
}

)"
