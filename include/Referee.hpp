#ifndef REFEREE_HPP
#define REFEREE_HPP

class AI;
class Human;
class InputManager;
namespace Game
{
    class Board;
}

class Referee
{
    public:
        Referee(AI* ai, InputManager* inpManager, Game::Board* board);

            void    processMove();
            void    takebackMove();
            bool    isGameOver() const;
            char    getWinnerChar() const;

    
    private:
        InputManager&       mInputManager;
        AI&                 mAi;

        bool                isHumanTurn;
        Game::Board&        mBoard;
        bool                mGameOver;
        char                mWinnerChar;

};

#endif