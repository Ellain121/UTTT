#include "Board.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

#include <iostream>


namespace Game
{


Board::Board(const sf::Vector2u& windowSize)
: mWindowSize(windowSize)
, mRect(50, 50, 500, 500)
, mGrid(sf::Vector2f(mRect.left, mRect.top), sf::Vector2f(mRect.width, mRect.height))
{
    
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mGrid);
}

void Board::update()
{

}

bool Board::processMove(const Game::Move& move)
{
    Position bigPos = getBigPosition(move.globalPos);
    Position smallPos = getSmallPosition(move.globalPos);
    bool success = false;

    if (mMoves.empty())
    {
        // mGrid.makeMove(bigPos, smallPos, getCharAccordingToTurn(move.crossesTurn));
        success = mGrid.makeMove(move);
    }
    else
    {
        auto& prevMove = mMoves.back();
        auto nextBigGridPos = getSmallPosition(prevMove.globalPos);
        if ((bigPos == nextBigGridPos) || mGrid.getSmallGrid(nextBigGridPos.y*3 + nextBigGridPos.x).isGridDone())
        {
            // mGrid.makeMove(bigPos, smallPos, getCharAccordingToTurn(move.crossesTurn));
            success = mGrid.makeMove(move);
        }
    }

    if (success)
    {
        mMoves.push_back(move);
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<Game::Position> Board::getAvailMoves() const
{
    // if board is clear and there is no prevMove
    if (mMoves.empty())
    {
        std::vector<Game::Position> availMoves(9*9);
        for (int h = 0; h < 9; ++h)
        for (int w = 0; w < 9; ++w)
        {
            availMoves[h*9 + w].y = h;
            availMoves[h*9 + w].x = w;
        }
        return availMoves;
    }


    std::vector<Game::Position> availMoves;
    auto& prevMove = mMoves.back();
    auto nextBigGridPos = getSmallPosition(prevMove.globalPos);
    // available moves at any free big grids
    if (mGrid.getSmallGrid(nextBigGridPos.y*3 + nextBigGridPos.x).isGridDone())
    {
        for (int i = 0; i < 9; ++i)
        {
            if (mGrid.getSmallGrid(i).isGridDone()) continue;

            auto getLocalPos = mGrid.getSmallGrid(i).getLocalAvailMoves();
            for (auto& localPos : getLocalPos)
            {
                availMoves.push_back(fromSmallToGlobal(localPos, i));
            }
            
        }
    }
    else // available only moves at 1 big grid
    {
        int bigIndx = nextBigGridPos.y * 3 + nextBigGridPos.x;
        auto getLocalPos = mGrid.getSmallGrid(bigIndx).getLocalAvailMoves();
        for (auto& localPos : getLocalPos)
        {
            availMoves.push_back(fromSmallToGlobal(localPos, bigIndx));
        }
    }
    std::cout << "----------------------------------\n";
    for (auto& availMove : availMoves)
    {
        // std::cout << "availMove: x: " << availMove.x << " " << availMove.y << std::endl;
    }
    std::cout << "----------------------------------\n";
    return availMoves;
}

bool Board::isGameOver() const
{
    return mGrid.isGridDone();
}

char Board::getWinnerChar() const
{
    return mGrid.getWinner();
}

void Board::takebackMove()
{
    mMoves.pop_back();
}

const std::vector<Game::Move>& Board::getMovesDone() const
{
    return mMoves;
}

Game::Position Board::getGlobalPosFromMouse(const sf::Vector2i& mPos)
{
    if (!mRect.contains(mPos))
    {
        return Game::Position(-1, -1);
    }

    auto visualAreas = mGrid.getVisualAreas();
    for (int i = 0; i < 9; ++i)
    {
        if (visualAreas[i].contains(mPos))
        {
            auto smallVisualAreas = mGrid.getSmallGridVisualAreas(i);
            for (int k = 0; k < 9; ++k)
            {
                if (smallVisualAreas[k].contains(mPos))
                {
                    int globalW = i % 3 * 3 + k % 3;
                    int globalH = i / 3 * 3 + k / 3;
                    return Game::Position(globalW, globalH);
                }
            }
        }
    }
    return Game::Position(-1, -1);
}

}