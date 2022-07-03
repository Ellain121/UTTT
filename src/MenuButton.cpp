#include "MenuButton.hpp"
#include "State.hpp"

MenuButton::MenuButton(State::Context context, SubTextures::MenuButton menuButtonType)
: Button(context)
, mMenuButtonType(menuButtonType)
{
    
    mSprite.setTexture(context.textures->get(Textures::ID::MenuButtons));
    mSprite.setTextureRect(getMenuButtonLocation(menuButtonType));
}

void MenuButton::setText(const std::string& text)
{
    mText.setString(text);
}

void MenuButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(mText, states);
    target.draw(mSprite, states);
}

void MenuButton::handleEvent(const sf::Event& event)
{
}