#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Component.hpp"
#include "State.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <functional>
#include <memory>

class Button : public Component
{
    public:
        typedef std::unique_ptr<Button>     Ptr;
        typedef std::function<void()>       Callback;

        enum Type
        {
            Normal,
            Selected,
            Pressed,
        };

    public:
                Button(State::Context context);
        
        void            setCallback(Callback callback);
        virtual void    setText(const std::string& text);
        
        virtual void    activate();
        virtual void    deactivate();

        virtual void    select();
        virtual void    deselect();

        virtual void    handleEvent(const sf::Event& event);


        bool            isSelectable() const;
    
    private:
        virtual void    draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void            changeButtonType(Type buttonType);
    
    private:
        Callback                mCallback;
        sf::Text                mText;
        sf::RectangleShape      mRectShape;

        const sf::Color         mButtonNormalColor;
        const sf::Color         mButtonSelectedColor;
        const sf::Color         mButtonPressedColor;

        // virtual void    handleEvent(const sf::Event& event);

        // virtual void    select();
        // virtual void    deselect();
};


#endif