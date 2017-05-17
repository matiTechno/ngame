#include <NGAME/renderer2d.hpp>
#include <NGAME/sprite.hpp>
#include <cstddef>
#include <glm/gtc/matrix_transform.hpp>

Renderer2d::Renderer2d():
    sh_sprite(
        #include "shaders/sprite.sh"
        , "sprite.sh")
{
    s_linear.set_parameter_i(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    s_linear.set_parameter_i(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    s_nearest.set_parameter_i(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    s_nearest.set_parameter_i(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

   glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vbo_instance), reinterpret_cast<const void*>(offsetof(Vbo_instance, tex_coords)));
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

   glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vbo_instance), reinterpret_cast<const void*>(offsetof(Vbo_instance, model)));
   glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vbo_instance),
                         reinterpret_cast<const void*>(offsetof(Vbo_instance, model) + sizeof(glm::vec4)));
   glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vbo_instance),
                         reinterpret_cast<const void*>(offsetof(Vbo_instance, model) + 2 * sizeof(glm::vec4)));
   glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vbo_instance),
                         reinterpret_cast<const void*>(offsetof(Vbo_instance, model) + 3 * sizeof(glm::vec4)));
}

void Renderer2d::flush(const glm::vec2& start, const glm::vec2& range, Sampl_type type) const
{
    if(batches.empty())
        return;

    if(type == Sampl_type::linear)
        s_linear.bind();
    else
        s_nearest.bind();

    sh_sprite.bind();
    auto matrix = glm::ortho(start.x, start.x + range.x, start.y + range.y, start.y);
    glUniformMatrix4fv(sh_sprite.get_uni_location("projection"), 1, GL_FALSE, &matrix[0][0]);

    for(auto& batch: batches)
    {
        if(batch.texture)
        {
            glUniform1i(sh_sprite.get_uni_location("is_texture"), 1);
            batch.texture->bind();
        }
        else
           glUniform1i(sh_sprite.get_uni_location("is_texture"), 0);

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
        if(sprite.texture != batches.end()->texture)
        {
            batches.emplace_back();
            batches.end()->texture = sprite.texture;
        }
    }
    else
    {
        batches.emplace_back();
        batches.back().texture = sprite.texture;
    }

    Vbo_instance inst;
    inst.color = sprite.color;
    inst.tex_coords = sprite.tex_coords;

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
