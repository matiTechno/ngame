#include <NGAME/gl/shader.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <NGAME/glad.h>

GLuint Shader::bound_id = 0;

Shader::Shader():
    GL_base([](GLuint& id){
    glDeleteProgram(id);
    if(bound_id == id)
        bound_id = 0;
})
{
    id = glCreateProgram();
}

Sh_part::Sh_part(GLenum sh_type):
    GL_base([](GLuint& id)
{
    glDeleteShader(id);
})
{
    id = glCreateShader(sh_type);
}

Shader::Shader(const std::string& filename, bool hot_reload):
    Shader()
{
    this->hot_reload = hot_reload;
    id_name = filename;

    std::error_code ec;
    ftt = fs::last_write_time(filename, ec);
    if(ec)
        std::cout << "shader, id_name = " + id_name + ": last_write_time failed" << std::endl;

    make_program(load_source_from_file(filename));
}

Shader::Shader(const std::string& source, const char* id_name):
    Shader()
{
    this->id_name = id_name;
    make_program(source);
}

void Shader::bind() const
{
    if(hot_reload)
        reload();

    if(bound_id != id)
    {
        bound_id = id;
        glUseProgram(id);
    }
}

GLint Shader::get_uni_location(const std::string& uni_name) const
{
    auto loc = uni_locations.find(uni_name);
    if(loc == uni_locations.end())
        std::cout << "shader, id_name = " + id_name + ": no active uniform = " + uni_name << std::endl;
    return loc->second;
}

void Shader::load_uni_locations() const
{
    GLint num_uniforms;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &num_uniforms);
    std::vector<char> uniform_name(256, 0);

    for(auto i = 0; i < num_uniforms; ++i)
    {
        GLint dum1;
        GLenum dum2;
        glGetActiveUniform(id, i, uniform_name.size(), nullptr, &dum1, &dum2,
                           uniform_name.data());

        GLint loc = glGetUniformLocation(id, uniform_name.data());
        uni_locations[uniform_name.data()] = loc;
    }
}

Sh_part Shader::compile_shader(GLenum sh_type, const std::string& gl_source) const
{
    Sh_part sh_part(sh_type);
    auto* c_str = gl_source.c_str();
    glShaderSource(sh_part.id, 1, &c_str, nullptr);
    glCompileShader(sh_part.id);
    return sh_part;
}

std::string Shader::load_source_from_file(const std::string& filename) const
{
    std::ifstream file(filename);
    if(!file)
        throw std::runtime_error("shader, load_form_file failed, filename = " + filename);

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

bool Shader::is_error(bool is_program, GLuint id, GLenum flag, const std::string& err_msg) const
{

    GLint success;
    if(is_program)
        glGetProgramiv(id, flag, &success);
    else
        glGetShaderiv(id, flag, &success);

    if(success == GL_FALSE)
    {
        GLint length;
        if(is_program)
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
        else
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        std::vector<GLchar> infoLog(length);

        if(is_program)
            glGetProgramInfoLog(id, length, nullptr, infoLog.data());
        else
            glGetShaderInfoLog(id, length, nullptr, infoLog.data());

        std::cout << err_msg + ':' << std::endl;
        std::cout << infoLog.data() << std::endl;

        return true;
    }
    return false;
}

void Shader::reload() const
{
    std::error_code ec;
    auto new_ftt = fs::last_write_time(id_name, ec);
    if(ec)
    {
        std::cout << "shader, id_name = " + id_name + ": last_write_time failed" << std::endl;
        return;
    }
    if(new_ftt != ftt)
    {
        ftt = new_ftt;
        make_program(load_source_from_file(id_name));
    }
}

struct CD
{
    CD(std::string::size_type pos, GLenum type, const std::string& name):
        pos(pos),
        type(type),
        name(name)
    {}

    std::string::size_type pos;
    GLenum type;
    std::string name;
};

void Shader::make_program(const std::string& source) const
{
    std::vector<Sh_part> sh_parts;
    sh_parts.reserve(3);
    bool comp_err = false;
    {
        std::vector<CD> indices;

        std::string str_v = "#VERTEX\n",
                str_g = "#GEOMETRY\n",
                str_f = "#FRAGMENT\n",
                str_c = "#COMPUTE\n";

        auto n = source.find(str_v);
        if(n != std::string::npos)
            indices.emplace_back(n, GL_VERTEX_SHADER, str_v);

        n = source.find(str_g);
        if(n != std::string::npos)
            indices.emplace_back(n, GL_GEOMETRY_SHADER, str_g);

        n = source.find(str_f);
        if(n != std::string::npos)
            indices.emplace_back(n, GL_FRAGMENT_SHADER, str_f);

        n = source.find(str_c);
        if(n != std::string::npos)
            indices.emplace_back(n, GL_COMPUTE_SHADER, str_c);

        std::sort(indices.begin(), indices.end(), [](CD& a, CD& b)
        {
            return  a.pos < b.pos;
        });

        for(auto i = indices.begin(); i != indices.end(); ++i)
        {
            std::string::size_type epos;
            if(i == indices.end() - 1)
                epos = std::string::npos;
            else
                epos = (i + 1)->pos;

            auto pos = i->pos + i->name.size();
            sh_parts.push_back(compile_shader(i->type, source.substr(pos, epos - pos)));
            comp_err |= is_error(false, sh_parts.back().id, GL_COMPILE_STATUS, "shader, id_name = " + id_name + ": " +
                                 i->name.substr(1, i->name.size() - 2) + " compilation failed");
        }
    }

    if(comp_err)
        return;

    for(auto& sh_part: sh_parts)
        glAttachShader(id, sh_part.id);

    glLinkProgram(id);

    for(auto& sh_part: sh_parts)
        glDetachShader(id, sh_part.id);

    if(is_error(true, id, GL_LINK_STATUS, "shader, id_name = " + id_name + ": linking failed"))
        return;

    std::cout << "shader, id_name = " + id_name + ": linking succeed" << std::endl;

    load_uni_locations();
}
