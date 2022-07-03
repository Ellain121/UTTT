#ifndef MENUBUTTON_HPP
#define MENUBUTTON_HPP

#include "Button.hpp"
#include "State.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

class MenuButton : public Button
{
    public:
        typedef std::unique_ptr<MenuButton>     Ptr;

    public:
                MenuButton(State::Context context, SubTextures::MenuButton menuButtonType);

        void    setText(const std::string& text);
        void    draw(sf::RenderTarget& target, sf::RenderStates states) const;

        void    handleEvent(const sf::Event& event);
    
    
    private:
        sf::Text                    mText;
        sf::Sprite                  mSprite;
        SubTextures::MenuButton     mMenuButtonType;

};

#endif