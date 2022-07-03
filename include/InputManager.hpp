#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "GameUtility.hpp"
#include "State.hpp"

#include <SFML/Window/Event.hpp>

namespace Game
{
class Board;
}

class InputManager
{
    public:
        InputManager(Game::Board* board, State::Context context);

        void                handleEvent(const sf::Event& event);
        Game::Position      getLastClickPosition();
    
    private:
        bool                mLastClickProcessed;
        Game::Board*        mBoard;
        Game::Position      mLastClickPos;
        State::Context      mContext;
};  


#endif