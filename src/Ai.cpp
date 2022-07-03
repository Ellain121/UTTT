#include "AI.hpp"
#include "Board.hpp"

#include <iostream>
#include <cassert>

AI::AI()
: mcts_ai()
{

}

Game::Position AI::think(const Game::Board& board)
{
    Game::Position lastPos = board.getLastMovePos();
    auto move_row = mcts_ai.think(lastPos.y, lastPos.x);

    return Game::Position(move_row.second, move_row.first);
}