#ifndef STATE_HPP
#define STATE_HPP

#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "StateIdentifiers.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class StateStack;

class State
{
    public:
        struct Context
        {
                    Context(sf::RenderWindow& window, const TextureHolder& textureHolder,
                                        const FontHolder& fontHolder);

            sf::RenderWindow*           window;
            const TextureHolder*        textures;
            const FontHolder*           fonts;
        };
    
    public:
        State(StateStack& stack, Context context);

        virtual void    draw() const = 0;
        virtual bool    update() = 0;
        virtual bool    handleEvent(const sf::Event& event) = 0;
    
    protected:
        void            requestStackPush(States::ID stateID);
        void            requestStackPop();
        void            requestStackClear();

        Context         getContext() const;
    
    private:
        StateStack*     mStateStack;
        Context         mContext;

};

#endif