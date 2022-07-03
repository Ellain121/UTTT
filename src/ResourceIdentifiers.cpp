#include "ResourceIdentifiers.hpp"

sf::IntRect getMenuButtonLocation(SubTextures::MenuButton menuButton)
{
    switch(menuButton)
    {
        case SubTextures::MenuButton::Play:
            return sf::IntRect(0, 0, 600, 200);

        case SubTextures::MenuButton::Start:
            return sf::IntRect(0, 208, 600, 200);

        case SubTextures::MenuButton::Load:
        case SubTextures::MenuButton::Resume:
        case SubTextures::MenuButton::Continue:
        case SubTextures::MenuButton::Menu:
        case SubTextures::MenuButton::Controls:
        case SubTextures::MenuButton::Settings:
            return sf::IntRect(609, 417, 600, 200);

        case SubTextures::MenuButton::Options:
        case SubTextures::MenuButton::NewGame:
        case SubTextures::MenuButton::Quit:
        case SubTextures::MenuButton::Exit:
            return sf::IntRect(1216, 625, 600, 200);
        case SubTextures::MenuButton::Back:
            break;
    }
    return sf::IntRect(0, 0, 0, 0);
}