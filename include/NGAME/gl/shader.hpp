#pragma once
#include "base.hpp"
#include "../del.hpp"
#include <memory>
#include <string>
#include <map>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

class Sh_part: public GL_base, public Del
{
public:
    friend class Shader;
    Sh_part(GLenum sh_type);
};

// shader source format:
// #VERTEX
// #GEOMETRY
// #FRAGMENT
// #COMPUTE

class Shader: public GL_base, public Del
{
public:
    Shader(const std::string& filename, bool hot_reload);
    Shader(const std::string& source, const std::string& id_name);

    void bind() const;

    GLint get_uni_location(const std::string& uni_name) const;

private:
    Shader();
    static GLuint bound_id;
    mutable std::map<std::string, GLint> uni_locations;
    bool hot_reload = false;
    mutable fs::file_time_type ftt;
    // when loaded from file it's filename
    std::string id_name;

    void make_program(const std::string& source) const;

    void load_uni_locations() const;
    Sh_part compile_shader(GLenum sh_type, const std::string& gl_source) const;
    std::string load_source_from_file(const std::string& filename) const;
    bool is_error(bool is_program, GLuint id, GLenum flag, const std::string& err_msg) const;
    void reload() const;
};
