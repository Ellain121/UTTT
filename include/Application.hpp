#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "StateStack.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class Application
{
    public:
                Application();
        
        void    run();
    
    private:
        void    registerStates();
        void    loadResources();

        void    processInput();
        void    update();
        void    render();

    private:
        sf::RenderWindow    mWindow;
        StateStack          mStateStack;
        TextureHolder       mTextureHolder;
        FontHolder          mFontHolder;
};

#endif