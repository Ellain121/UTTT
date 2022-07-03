#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
    public:
        typedef std::unique_ptr<Component>  Ptr;

    public:
                    Component();

        virtual bool    isSelectable() const = 0;
        bool            isSelected() const;
        virtual void    select();
        virtual void    deselect();

        virtual bool    isActive() const;
        virtual void    activate();
        virtual void    deactiate();

        virtual void    handleEvent(const sf::Event& event) = 0;
        

    private:
        bool        mIsSelected;
        bool        mIsActive;
    
};

#endif