#ifndef RESOURCEIDENTIFIERS_HPP
#define RESOURCEIDENTIFIERS_HPP

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

namespace sf
{
    class Texture;
    class Font;
}

namespace Textures
{
    enum ID
    {
        MenuButtons
    };
}

namespace SubTextures
{
    enum MenuButton
    {
        Play,
        Start,
        Load,
        Resume,
        Continue,
        Menu,
        Controls,
        Settings,
        Options,
        NewGame,
        Quit,
        Exit,
        Back
    };
}


namespace Fonts
{
    enum ID
    {
        Main
    };
}

sf::IntRect getMenuButtonLocation(SubTextures::MenuButton menuButton);

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>   TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>         FontHolder;

#endif