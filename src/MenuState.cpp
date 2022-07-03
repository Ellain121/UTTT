#include "MenuState.hpp"

#include "Button.hpp"

MenuState::MenuState(StateStack& stack, State::Context context)
: State(stack, context)
, mContainer(context)
{
    sf::Vector2u size = context.window->getSize();

    Button::Ptr playButton = std::make_unique<Button>(context);
    playButton->setPosition(size.x / 2, 250);
    playButton->setText("Play");
    playButton->setCallback([this](){
        requestStackPop();
        requestStackPush(States::ID::Game);
    });

    Button::Ptr settingsButton = std::make_unique<Button>(context);
    settingsButton->setPosition(size.x / 2, 325);
    settingsButton->setText("Settings");

    Button::Ptr exitButton = std::make_unique<Button>(context);
    exitButton->setPosition(size.x / 2, 400);
    exitButton->setText("Exit");
    exitButton->setCallback([this](){
        requestStackPop();
    });

    mContainer.pack(std::move(playButton));
    mContainer.pack(std::move(settingsButton));
    mContainer.pack(std::move(exitButton));
}

bool MenuState::handleEvent(const sf::Event& event)
{
    mContainer.handleEvent(event);
    return false;
}

bool MenuState::update()
{
    mContainer.update();
    return false;
}

void MenuState::draw() const
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(mContainer);
}
