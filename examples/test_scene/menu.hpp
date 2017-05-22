#include <NGAME/scene.hpp>
class Font;

class Menu: public Scene
{
public:
    Menu(const Font& font);

    void process_input() override;

    void render() override;

private:
    const Font& font;
};
