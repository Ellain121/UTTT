#include "Referee.hpp"

#include "AI.hpp"
#include "InputManager.hpp"
#include "Board.hpp"

#include <SFML/System/Sleep.hpp>
sf::Time sleepTime = sf::seconds(1);

Referee::Referee(AI* ai, InputManager* inpManager, Game::Board* board)
: mInputManager(*inpManager)
, mAi(*ai)
, mBoard(*board)
, isHumanTurn(true)
, mGameOver(false)
, mWinnerChar('-')
{
    
}

void Referee::processMove()
{
    if (isHumanTurn)
    {
        auto lastClickPos = mInputManager.getLastClickPosition();
        if (lastClickPos.x == -1) return;

        bool success = mBoard.processMove(Game::Move(lastClickPos, true));
        if (success)
            isHumanTurn = !isHumanTurn;
        else
            std::cout << "Human move failed: x: " << lastClickPos.x << " " << lastClickPos.y << std::endl;
        
        
    }
    else
    {
        auto boardPos = mAi.think(mBoard);
        sf::sleep(sleepTime);
        bool success = mBoard.processMove(Game::Move(boardPos, false));
        if (success)
            isHumanTurn = !isHumanTurn;
        else
            std::cout << "AI move failed: x: " << boardPos.x << " " << boardPos.y << std::endl;
    }

    if (mBoard.isGameOver())
    {
        mGameOver = true;
        mWinnerChar = mBoard.getWinnerChar();
    }
}

bool Referee::isGameOver() const
{
    return mGameOver;
}

char Referee::getWinnerChar() const
{
    return mWinnerChar;
}

void Referee::takebackMove()
{
    mBoard.takebackMove();
}