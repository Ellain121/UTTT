#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "State.hpp"
#include "Board.hpp"
#include "InputManager.hpp"
#include "Referee.hpp"
#include "GamePanel.hpp"
#include "AI.hpp"

class GameState : public State
{
    public:
        GameState(StateStack& stack, State::Context context);

        void    draw() const;
        bool    update();
        bool    handleEvent(const sf::Event& event);

    private:
        Game::Board     mBoard;
        InputManager    mInputManager;
        AI              mAI;
        Referee         mReferee;
        GamePanel       mGamePanel;
};

#endif