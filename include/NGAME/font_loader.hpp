#pragma once
#include <string>
#include <memory>
#include <vector>
class Font;

class Font_loader
{
public:
    Font load_font(const std::string& filename, int size_px) const;

private:
    std::vector<unsigned char> load_ttf(const std::string& filename) const;
};
