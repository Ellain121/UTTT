#include "GameState.hpp"

GameState::GameState(StateStack& stack, State::Context context)
: State(stack, context)
, mBoard(context.window->getSize())
, mInputManager(&mBoard, context)
, mAI()
, mReferee(&mAI, &mInputManager, &mBoard)
, mGamePanel(&mBoard, &mInputManager)
{

}

void GameState::draw() const
{
    auto& window = *getContext().window;

    window.draw(mGamePanel);
    window.draw(mBoard);
}

bool GameState::update()
{
    mReferee.processMove();
    if (mReferee.isGameOver())
    {
        throw std::logic_error(std::string("GAME IS OVER; Winner: ") + mReferee.getWinnerChar());
    }
    // mGamePanel.processPossibleClicks();

    return false;
}

bool GameState::handleEvent(const sf::Event& event)
{
    mInputManager.handleEvent(event);

    return false;
}   