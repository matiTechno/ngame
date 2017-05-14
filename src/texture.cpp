#include <NGAME/gl/texture.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::pair<GLuint, GLuint> Texture::bound_id{0, 0};
std::pair<GLuint, GLuint> Sampler::bound_id{0, 0};

Texture::Texture(const std::string& filename, bool sRGB, GLsizei levels):
    Texture()
{
    auto* data = stbi_load(filename.c_str(), &size.x, &size.y, nullptr, 4);
    if(!data)
        throw std::runtime_error("stbi_load failed, filename = " + filename);

    bind();

    GLenum internal_format;
    if(sRGB)
        internal_format = GL_SRGB8_ALPHA8;
    else
        internal_format = GL_RGBA8;

    glTexStorage2D(GL_TEXTURE_2D, levels, internal_format, size.x, size.y);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, data);

    if(levels > 1)
        glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

Texture::Texture(GLenum internal_format, GLsizei width, GLsizei height, GLsizei levels):
    Texture()
{
    bind();
    glTexStorage2D(GL_TEXTURE_2D, levels, internal_format, width, height);
}

Texture::Texture():
    Del([this]()
{
    glDeleteTextures(1, &id);
    if(bound_id.first == id)
        bound_id.first = 0;
})
{
    glGenTextures(1, &id);
}

void Texture::bind(GLenum target, GLuint unit) const
{
    if(id != bound_id.first || unit != bound_id.second)
    {
        bound_id.first = id;
        bound_id.second = unit;
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(target, id);
    }
}

glm::ivec2 Texture::get_size() const
{
    return size;
}

Sampler::Sampler():
    Del([this](){
    glDeleteSamplers(1, &id);
    if(bound_id.first == id)
        bound_id.first = 0;
})
{
    glGenSamplers(1, &id);
}
void Sampler::bind(GLuint unit) const
{
    if(id != bound_id.first || unit != bound_id.second)
    {
        bound_id.first = id;
        bound_id.second = unit;
        glBindSampler(unit, id);
    }
}

void Sampler::set_parameter_i(GLenum pname, GLint param) const
{
    glSamplerParameteri(id, pname, param);
}
