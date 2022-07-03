#include "Button.hpp"
#include "Utility.hpp"

Button::Button(State::Context context)
: mText("", context.fonts->get(Fonts::ID::Main), 16)
, mCallback()
, mButtonNormalColor(sf::Color(150, 150, 150))
, mButtonSelectedColor(sf::Color(50, 50, 50))
, mButtonPressedColor((sf::Color(100, 100, 0)))
{
    mRectShape.setSize(sf::Vector2f(200, 75));
    mRectShape.setOutlineThickness(2.f);
    mRectShape.setOutlineColor(sf::Color::Black);
    centerOrigin(mRectShape);
    changeButtonType(Normal);
}

void Button::setCallback(Callback callback)
{
    mCallback = std::move(callback);
}

void Button::setText(const std::string& text)
{
    mText.setString(text);
    centerOrigin(mText);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(mRectShape, states);
    target.draw(mText, states);
}

void Button::select()
{
    Component::select();

    changeButtonType(Selected);
}

void Button::deselect()
{
    Component::deselect();

    changeButtonType(Normal);
}

void Button::activate()
{
    Component::activate();

    if (mCallback)
    {
        mCallback();
    }

    deactiate();
}

void Button::deactivate()
{
    Component::deactiate();
}

void Button::handleEvent(const sf::Event& event)
{

}

void Button::changeButtonType(Type buttonType)
{
    if (buttonType == Type::Normal)
        mRectShape.setFillColor(mButtonNormalColor);

    if (buttonType == Type::Selected)
        mRectShape.setFillColor(mButtonSelectedColor);

    if (buttonType == Type::Pressed)
        mRectShape.setFillColor(mButtonPressedColor);
}

bool Button::isSelectable() const
{
    return true;
}