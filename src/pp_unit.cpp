#include <NGAME/pp_unit.hpp>

PP_unit::PP_unit(int w, int h):
    texs{Texture(GL_RGBA8, w, h), Texture(GL_RGBA8, w, h)},
    w(w),
    h(h),
    has_finished(true)
{
    float vertices[] = {
        -1.f, -1.f, 0.f, 0.f,
        1.f, -1.f, 1.f, 0.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        -1.f, 1.f, 0.f, 1.f,
        -1.f, -1.f, 0.f, 0.f};

    bo_quad.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vao.bind();
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    for(auto i = 0; i < 2; ++i)
    {
        fbos[i].bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texs[i].id, 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PP_unit::start() const
{
    assert(has_finished);
    has_finished = false;

    glScissor(0, 0, w, h);

    for(auto i = 0; i < 2; ++i)
    {
        fbos[i].bind();
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glScissor(scene_gl_coords.x, scene_gl_coords.y, scene_gl_coords.z, scene_gl_coords.w);

    bound_fbo = 1;
}

void PP_unit::render(bool end, const Shader& shader) const
{
    assert(!has_finished);

    glViewport(0, 0, w, h);

    shader.bind();
    vao.bind();

    texs[bound_fbo].bind();

    if(end)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        has_finished = true;
    }
    else
    {
        bound_fbo = !bound_fbo;
        fbos[bound_fbo].bind();
    }

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glViewport(scene_gl_coords.x, scene_gl_coords.y, scene_gl_coords.z, scene_gl_coords.w);
}

void PP_unit::set_new_size(int w, int h)
{
    if(w == this->w && h == this->h)
        return;

    this->w = w;
    this->h = h;

    for(auto i = 0; i < 2; ++i)
    {
        texs[i] = Texture(GL_RGBA8, w, h);
        fbos[i].bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texs[i].id, 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PP_unit::set_scene(const glm::ivec4& scene_gl_coords)
{
    this->scene_gl_coords = scene_gl_coords;
}
