#ifndef AI_HPP
#define AI_HPP

#include "Board.hpp"

class AI
{
    public:
        AI();

        Game::Position      think(const Game::Board& board);
};

#endif