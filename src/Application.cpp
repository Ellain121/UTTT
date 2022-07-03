#include "Application.hpp"

#include "MenuState.hpp"
#include "GameState.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Event.hpp>

sf::Time slowDown = sf::microseconds(700);
constexpr int WIDTH = 1024;
constexpr int HEIGHT = 768;

Application::Application()
: mWindow(sf::VideoMode(WIDTH, HEIGHT), "Minesweeper")
, mStateStack(State::Context(mWindow, mTextureHolder, mFontHolder))
{
    registerStates();
    loadResources();

    mStateStack.pushState(States::ID::Menu);
}

void Application::registerStates()
{
    mStateStack.registerState<MenuState>(States::ID::Menu);
    mStateStack.registerState<GameState>(States::ID::Game);
}

void Application::loadResources()
{
    // mTextureHolder.load(Textures::ID::MenuButtons, "../data/Menu_Buttons.png");
    mFontHolder.load(Fonts::ID::Main, "../data/open-sans.regular.ttf");
}

void Application::run()
{
    do
    {
        processInput();
        update();
        render();
        sf::sleep(slowDown);
    } while (!mStateStack.isEmpty());
}

void Application::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            mWindow.close();
        mStateStack.handleEvent(event);
    }
}

void Application::update()
{
    mStateStack.update();
}

void Application::render()
{
    mWindow.clear();

    mStateStack.draw();

    mWindow.display();
}
