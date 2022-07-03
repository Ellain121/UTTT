#include "InputManager.hpp"
#include "Board.hpp"

InputManager::InputManager(Game::Board* board, State::Context context)
: mBoard(board)
, mLastClickPos(-1, -1)
, mContext(context)
, mLastClickProcessed(true)
{

}

void InputManager::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonReleased)
    {
        sf::RenderWindow& window = *mContext.window;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        mLastClickPos = mBoard->getGlobalPosFromMouse(mousePos);
        mLastClickProcessed = false;
        std::cout << "gPos: x: " << mLastClickPos.x << " y: " << mLastClickPos.y << std::endl;

    }
}

Game::Position InputManager::getLastClickPosition()
{
    if (mLastClickProcessed == false)
    {
        mLastClickProcessed = true;
        return mLastClickPos;
    }
    else
    {
        return Game::Position(-1, -1);
    }
}