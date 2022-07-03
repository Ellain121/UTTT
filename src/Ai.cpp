#include "AI.hpp"
#include "Board.hpp"

#include <iostream>
#include <cassert>

AI::AI()
{

}

Game::Position AI::think(const Game::Board& board)
{
    auto availMoves = board.getAvailMoves();
    std::cout << "availMoves.size() = " << availMoves.size() << std::endl;
    assert(!availMoves.empty());
    return availMoves[0];
}