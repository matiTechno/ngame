#include <NGAME/renderer2d.hpp>
#include <NGAME/sprite.hpp>
#include <cstddef>
#include <glm/gtc/matrix_transform.hpp>
#include <NGAME/text.hpp>
#include <NGAME/font.hpp>
#include <NGAME/glad.h>

Renderer2d::Renderer2d():
    sh_sprite(
        #include "shaders/sprite.sh"
        , "sprite.sh")
{
    sampl_linear.set_parameter_i(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    sampl_linear.set_parameter_i(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    sampl_nearest.set_parameter_i(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    sampl_nearest.set_parameter_i(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    float vertices[] = {
        0.f, 0.f, 0.f, 0.f,
        1.f, 0.f, 1.f, 0.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        0.f, 1.f, 0.f, 1.f,
        0.f, 0.f, 0.f, 0.f};

    bo_quad.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vao.bind();
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    bo_sprite.bind(GL_ARRAY_BUFFER);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vbo_instance), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vbo_instance),
                          reinterpret_cast<const void*>(offsetof(Vbo_instance, tex_coords)));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    // model matrix

    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);
    glEnableVertexAttribArray(5);
    glVertexAttribDivisor(5, 1);
    glEnableVertexAttribArray(6);
    glVertexAttribDivisor(6, 1);

    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vbo_instance),
                          reinterpret_cast<const void*>(offsetof(Vbo_instance, model)));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vbo_instance),
                          reinterpret_cast<const void*>(offsetof(Vbo_instance, model) + sizeof(glm::vec4)));
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vbo_instance),
                          reinterpret_cast<const void*>(offsetof(Vbo_instance, model) + 2 * sizeof(glm::vec4)));
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vbo_instance),
                          reinterpret_cast<const void*>(offsetof(Vbo_instance, model) + 3 * sizeof(glm::vec4)));
}

void Renderer2d::set_projection(const glm::vec2& start, const glm::vec2& range) const
{
    sh_sprite.bind();
    auto matrix = glm::ortho(start.x, start.x + range.x, start.y + range.y, start.y);
    glUniformMatrix4fv(sh_sprite.get_uni_location("projection"), 1, GL_FALSE, &matrix[0][0]);
}

void Renderer2d::flush(Sampl_type type) const
{
    if(batches.empty())
        return;

    if(type == Sampl_type::linear)
        sampl_sprite = &sampl_linear;
    else
        sampl_sprite = &sampl_nearest;

    sh_sprite.bind();

    for(auto& batch: batches)
    {
        if(batch.texture)
        {
            batch.texture->bind();
            if(batch.is_sprite)
            {
                glUniform1i(sh_sprite.get_uni_location("type"), Rend_t::sp_tex);
                sampl_sprite->bind();
            }
            else
            {
                 glUniform1i(sh_sprite.get_uni_location("type"), Rend_t::font);
                 sampl_linear.bind();
            }
        }
        else
            glUniform1i(sh_sprite.get_uni_location("type"), Rend_t::sp_no_tex);

        bo_sprite.bind(GL_ARRAY_BUFFER);
        glBufferData(GL_ARRAY_BUFFER, batch.vbo_data.size() * sizeof(Vbo_instance), batch.vbo_data.data(), GL_STREAM_DRAW);

        vao.bind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, batch.vbo_data.size());
    }

    batches.clear();
}

void Renderer2d::render(const Sprite& sprite) const
{
    if(batches.size())
    {
        if(sprite.texture != batches.back().texture || batches.back().is_sprite == false)
            goto add_batch;
    }
    else
    {
        add_batch:
        batches.emplace_back();
        batches.back().texture = sprite.texture;
        batches.back().is_sprite = true;
    }

    Vbo_instance inst;
    inst.color = sprite.color;

    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3(sprite.pos, 0.f));
    if(sprite.rotation != 0.f)
    {
        model = glm::translate(model, glm::vec3(sprite.rotation_point, 0.f));
        model = glm::rotate(model, sprite.rotation, glm::vec3(0.f, 0.f, -1.f));
        model = glm::translate(model, glm::vec3(-sprite.rotation_point, 0.f));
    }
    model = glm::scale(model, glm::vec3(sprite.size, 1.f));
    inst.model = std::move(model);

    if(sprite.texture)
    {
        inst.tex_coords.x = float(sprite.tex_coords.x) / sprite.texture->get_size().x;
        inst.tex_coords.y = float(sprite.tex_coords.y) / sprite.texture->get_size().y;
        inst.tex_coords.z = float(sprite.tex_coords.z) / sprite.texture->get_size().x;
        inst.tex_coords.w = float(sprite.tex_coords.w) / sprite.texture->get_size().y;
    }

    batches.back().vbo_data.push_back(std::move(inst));
}

void Renderer2d::render(const Text& text) const
{
    if(batches.size())
    {
        if(&text.font->get_atlas() != batches.back().texture || batches.back().is_sprite)
            goto add_batch;
    }
    else
    {
        add_batch:
        batches.emplace_back();
        batches.back().texture = &text.font->get_atlas();
        batches.back().is_sprite = false;
    }

    glm::vec2 pen_pos;
    if(text.rotation == 0.f && text.scale == 1.f && text.snap_to_grid)
        pen_pos = glm::ivec2(text.pos + 0.5f);
    else
        pen_pos = text.pos;

    for(auto c: text.text)
    {
        if(c == '\n')
        {
            pen_pos.x = text.pos.x;
            pen_pos.y += text.font->get_new_line_space() * text.scale;
            continue;
        }
        Vbo_instance inst;
        inst.color = text.color;

        auto& glyph = text.font->get_glyph(c);

        auto glyph_pos = glm::vec2(pen_pos.x + glyph.bearing.x * text.scale, pen_pos.y - glyph.bearing.y * text.scale);
        glm::vec2 glyph_size(glyph.tex_coords.z * text.scale, glyph.tex_coords.w * text.scale);

        glm::mat4 model(1.f);
        model = glm::translate(model, glm::vec3(glyph_pos, 0.f));
        if(text.rotation != 0.f)
        {
            model = glm::translate(model, glm::vec3(text.rotation_point - (glyph_pos - text.pos), 0.f));
            model = glm::rotate(model, text.rotation, glm::vec3(0.f, 0.f, -1.f));
            model = glm::translate(model, glm::vec3(-text.rotation_point + (glyph_pos - text.pos), 0.f));
        }
        model = glm::scale(model, glm::vec3(glyph_size, 1.f));
        inst.model = std::move(model);

        inst.tex_coords.x = float(glyph.tex_coords.x) / text.font->get_atlas().get_size().x;
        inst.tex_coords.y = float(glyph.tex_coords.y) / text.font->get_atlas().get_size().y;
        inst.tex_coords.z = float(glyph.tex_coords.z) / text.font->get_atlas().get_size().x;
        inst.tex_coords.w = float(glyph.tex_coords.w) / text.font->get_atlas().get_size().y;

        batches.back().vbo_data.push_back(std::move(inst));

        pen_pos.x += glyph.advance * text.scale;
    }
}
