#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "State.hpp"
#include "Container.hpp"

class MenuState : public State
{
    public:
            MenuState(StateStack& stack, State::Context context);
    
        void        draw() const;
        bool        update();
        bool        handleEvent(const sf::Event& event);
    
    private:
        Container       mContainer;
};

#endif