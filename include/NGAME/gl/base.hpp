#pragma once
#include "../glad.h"
#include <utility>

class GL_base
{
protected:
    template<typename T>
    GL_base(T del):
        del(std::forward<T>(del))
    {}

    ~GL_base()
    {
        if(del)
            del(id);
    }

    GL_base(const GL_base&) = delete;
    GL_base& operator=(const GL_base&) = delete;

    GL_base(GL_base&& rhs):
        id(rhs.id),
        del(rhs.del)
    {
        rhs.del = nullptr;
    }

    GL_base& operator=(GL_base&& rhs)
    {
       if(this != &rhs)
       {
           del(id);
           id = rhs.id;
           rhs.del = nullptr;

       }
       return *this;
    }

    mutable GLuint id;

private:
    void (*del)(GLuint&);
};
