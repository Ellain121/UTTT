#ifndef AI_HPP
#define AI_HPP

#include "Board.hpp"
#include "MCTS_AI.hpp"

class AI
{
    public:
        AI();

        Game::Position      think(const Game::Board& board);
        
    private:
        MCTS_AI     mcts_ai;
};

#endif