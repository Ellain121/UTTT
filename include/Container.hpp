#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "Component.hpp"
#include "State.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <vector>

class Container : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
    public:
                Container(State::Context context);

        void    draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void    handleEvent(const sf::Event& event);
        void    update();

        void    pack(Component::Ptr component);
    
    private:
        bool    hasSelection() const;
        void    select(std::size_t index);
        void    selectNext();
        void    selectPrevious();

    private:
        std::vector<Component::Ptr>     mComponents;
        int                             mSelected;
};


#endif