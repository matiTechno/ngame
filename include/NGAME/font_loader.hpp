#pragma once
#include <string>
#include <memory>
class Font;

class Font_loader
{
public:
    Font load_font(const std::string& filename, int size_px) const;
};
