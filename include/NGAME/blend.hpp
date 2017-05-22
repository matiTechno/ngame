#pragma once

typedef unsigned int GLenum;

class Blend
{
public:
    static void set(GLenum src, GLenum dst);

    static void set_default();
};
