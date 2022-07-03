#include "GameUtility.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/CircleShape.hpp>

// debug
#include <cassert>
#include <iostream>


namespace Game
{

Position::Position(int x, int y)
: x(x), y(y)
{
}

bool Position::operator==(const Position& pos)
{
    return pos.x == x && pos.y == y;
}

Move::Move(const Position& pos, bool crossesTurn)
: globalPos(pos)
, crossesTurn(crossesTurn)
{
}

Game::Position getBigPosition(const Game::Position& globalPos)
{
    return Game::Position(globalPos.x / 3, globalPos.y / 3);
}

Game::Position getSmallPosition(const Game::Position& globalPos)
{
    return Game::Position(globalPos.x % 3, globalPos.y % 3);
}

Game::Position fromSmallToGlobal(const Game::Position smallPos, int bigIndx)
{
    return Game::Position(bigIndx%3 * 3 + smallPos.x, bigIndx/3 * 3 + smallPos.y);
}

int getGlobalIndxFromGlobalPos(const Game::Position& globalPos)
{
    return globalPos.y / 3 * 3 + globalPos.x / 3;
}

int getLocalIndxFromGlobalPos(const Game::Position& globalPos)
{
    return globalPos.y % 3 * 3 + globalPos.x % 3;
}

char getCharAccordingToTurn(bool crossesTurn)
{
    if (crossesTurn) return Game::cross;
    else return Game::naught;
}

//------------------------------------------------------------

SmallGrid::Visual::Visual(const sf::Vector2f& pos, const sf::Vector2f& size)
: gridRect(pos, size)
{
    const sf::Vector2f areaSize(size.x * 0.32f, size.y * 0.32f);
    for (int h = 0; h < 3; ++h)
    for (int w = 0; w < 3; ++w)
    {
        sf::Vector2i aPos(pos.x + 0.34f * (float)w * size.x, pos.y + 0.34f * (float)h * size.y);
        areas[h*3 + w] = sf::Rect<int>(aPos, sf::Vector2i(areaSize.x, areaSize.y));   
    }

    horizontalLines[0].setPosition(pos.x, pos.y + size.y * 0.32f);
    horizontalLines[0].setSize(sf::Vector2f(size.x, size.y * 0.02f));
    horizontalLines[0].setFillColor(sf::Color::Green);
    horizontalLines[1].setPosition(pos.x, pos.y + size.y * 0.66f);
    horizontalLines[1].setSize(sf::Vector2f(size.x, size.y * 0.02f));
    horizontalLines[1].setFillColor(sf::Color::Green);

    verticalLines[0].setPosition(pos.x + size.x * 0.32f, pos.y);
    verticalLines[0].setSize(sf::Vector2f(size.x * 0.02f, size.y));
    verticalLines[0].setFillColor(sf::Color::Green);
    verticalLines[1].setPosition(pos.x + size.x * 0.66f, pos.y);
    verticalLines[1].setSize(sf::Vector2f(size.x * 0.02f, size.y));
    verticalLines[1].setFillColor(sf::Color::Green);
}

SmallGrid::SmallGrid(const sf::Vector2f& pos, const sf::Vector2f& size)
: gridWinnerChar('-')
, visual(pos, size)
, availableToPlay(true)
{
    for (int h = 0; h < 3; ++h)
    for (int w = 0; w < 3; ++w)
    {
        grid[h][w] = '-';
    }
}

bool SmallGrid::makeMove(const Position& smallPos, char ch)
{
    // debug
    assert(smallPos.x >= 0 && smallPos.x < 3 && smallPos.y >= 0 && smallPos.y < 3);
    // assert(grid[smallPos.y][smallPos.x] == empty);
    assert(gridWinnerChar == empty);
    auto& cell = grid[smallPos.y][smallPos.x];
    if (cell != empty) return false;

    cell = ch;

    gridWinnerChar = getWinnerChar();
    return true;
}

void SmallGrid::setAvailability(bool isAvailable)
{
    sf::Color green = sf::Color::Green;
    if (isAvailable)
    {
        visual.verticalLines[0].setFillColor(green);
        visual.verticalLines[1].setFillColor(green);
        visual.horizontalLines[0].setFillColor(green);
        visual.horizontalLines[1].setFillColor(green);
    }
    else
    {
        visual.verticalLines[0].setFillColor(sf::Color(green.r*0.6f, green.g*0.6f, green.b*0.6f));
        visual.verticalLines[1].setFillColor(sf::Color(green.r*0.6f, green.g*0.6f, green.b*0.6f));
        visual.horizontalLines[0].setFillColor(sf::Color(green.r*0.6f, green.g*0.6f, green.b*0.6f));
        visual.horizontalLines[1].setFillColor(sf::Color(green.r*0.6f, green.g*0.6f, green.b*0.6f));
    }
    availableToPlay = isAvailable;
}

bool SmallGrid::isGridDone() const
{
    bool full = true;
    for (int h = 0; h < 3; ++h)
    for (int w = 0; w < 3; ++w)
    {
        if (grid[h][w] == empty)
        {
            full = false;
            break;
        }
    }
    return (gridWinnerChar != empty || full);
}

char SmallGrid::getWinner() const
{
    // debug
    // assert(gridWinnerChar != empty);

    return gridWinnerChar;
}

std::vector<Game::Position> SmallGrid::getLocalAvailMoves() const
{
    std::vector<Game::Position> availMoves;

    for (int h = 0; h < 3; ++h)
    for (int w = 0; w < 3; ++w)
    {
        if (grid[h][w] == empty)
        {
            availMoves.push_back(Game::Position(w, h));
        }
    }
    return availMoves;
}

sf::Rect<int>* SmallGrid::getVisualAreas()
{
    return visual.areas;
}

char SmallGrid::getWinnerChar()
{
    // horizontal check
    for (int h = 0; h < 3; ++h)
    {
        if (grid[h][0] == empty) continue;

        if (grid[h][0] == grid[h][1] && grid[h][1] == grid[h][2])
            return grid[h][0];       
    }

    // vertical check
    for (int w = 0; w < 3; ++w)
    {
        if (grid[0][w] == empty) continue;

        if (grid[0][w] == grid[1][w] && grid[1][w] == grid[2][w])
            return grid[0][w];       
    }

    // diagonal check
    if (grid[1][1] != empty)
    {
        // diagonal 1
        if (grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2])
            return grid[1][1];
        // diagonal 2
        if (grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0])
            return grid[1][1];
    }

    return empty;
}

void SmallGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // for (int i = 0; i < 9; ++i)
        // target.draw(visual.areas[i]);
    if (gridWinnerChar != empty) // winner, draw winner char
    {
        auto& grid = visual.gridRect;
        auto gridSize = grid.getSize();
        auto gridPos = grid.getPosition();
        if (gridWinnerChar == cross)
        {
            sf::RectangleShape rect1;
            sf::RectangleShape rect2;
            rect1.setSize(sf::Vector2f(gridSize.x * 0.07f, gridSize.y));
            rect2.setSize(sf::Vector2f(gridSize.x * 0.07f, gridSize.y));
            rect1.setPosition(sf::Vector2f(gridPos.x + gridSize.x/2.f, gridPos.y + gridSize.y/2.f));
            rect2.setPosition(sf::Vector2f(gridPos.x + gridSize.x/2.f, gridPos.y + gridSize.y/2.f));
            sf::Color cyan = sf::Color::Cyan;
            rect1.setFillColor(cyan);
            rect2.setFillColor(cyan);
            rect1.setRotation(45.f);
            rect2.setRotation(-45.f);
            centerOrigin(rect1);
            centerOrigin(rect2);

            target.draw(rect1);
            target.draw(rect2);
        }
        else
        {
            float thickness = 7.f;
            sf::CircleShape circle;
            circle.setRadius(gridSize.x / 2.75f);
            circle.setPosition(sf::Vector2f(gridPos.x + gridSize.x/2.f + thickness, 
                                                gridPos.y + gridSize.y/2.f + thickness));
            circle.setOutlineThickness(thickness);
            circle.setFillColor(sf::Color::Black);
            circle.setOutlineColor(sf::Color::Cyan);
            centerOrigin(circle);

            target.draw(circle);
        }
    }
    else // no winner, draw full grid
    {
        for (int h = 0; h < 3; ++h)
        for (int w = 0; w < 3; ++w)
        {
            if (grid[h][w] != empty)
            {
                if (grid[h][w] == naught)
                {
                    sf::CircleShape circle;
                    auto areaSize = visual.areas[h*3 + w].getSize();
                    auto areaPos = visual.areas[h*3 + w].getPosition();
                    float thickness = 3.f;
                    circle.setRadius(areaSize.x / 2.75f);
                    circle.setPosition(sf::Vector2f(areaPos.x + areaSize.x/2.f + thickness, 
                                                        areaPos.y + areaSize.y/2.f + thickness));
                    circle.setOutlineThickness(thickness);
                    circle.setFillColor(sf::Color::Black);
                    sf::Color cyan = sf::Color::Cyan;
                    if (availableToPlay)
                    {
                        circle.setOutlineColor(cyan);
                    }
                    else
                    {
                        circle.setOutlineColor(sf::Color(cyan.r*0.6f, cyan.g*0.6f, cyan.b*0.6f));
                    }
                    centerOrigin(circle);
                    
                    target.draw(circle);
                }
                else
                {
                    sf::RectangleShape rect1;
                    sf::RectangleShape rect2;
                    auto areaSize = visual.areas[h*3 + w].getSize();
                    auto areaPos = visual.areas[h*3 + w].getPosition();
                    rect1.setSize(sf::Vector2f(areaSize.x * 0.1f, areaSize.y));
                    rect2.setSize(sf::Vector2f(areaSize.x * 0.1f, areaSize.y));
                    rect1.setPosition(sf::Vector2f(areaPos.x + areaSize.x/2.f, areaPos.y + areaSize.y/2.f));
                    rect2.setPosition(sf::Vector2f(areaPos.x + areaSize.x/2.f, areaPos.y + areaSize.y/2.f));
                    sf::Color cyan = sf::Color::Cyan;
                    if (availableToPlay)
                    {
                        rect1.setFillColor(cyan);
                        rect2.setFillColor(cyan);
                    }
                    else
                    {
                        rect1.setFillColor(sf::Color(cyan.r*0.6f, cyan.g*0.6f, cyan.b*0.6f));
                        rect2.setFillColor(sf::Color(cyan.r*0.6f, cyan.g*0.6f, cyan.b*0.6f));
                    }
                    rect1.setRotation(45.f);
                    rect2.setRotation(-45.f);
                    centerOrigin(rect1);
                    centerOrigin(rect2);

                    target.draw(rect1);
                    target.draw(rect2);
                }
            }
        }
        
        for (int i = 0; i < 2; ++i)
        {
            target.draw(visual.verticalLines[i]);
            target.draw(visual.horizontalLines[i]);
        }
    }
}

//-----------------------------------------------------------------
BigGrid::Visual::Visual(const sf::Vector2f& pos, const sf::Vector2f& size)
: gridRect(pos, size)
{
    const sf::Vector2f areaSize(size.x * 0.32f, size.y * 0.32f);
    for (int h = 0; h < 3; ++h)
    for (int w = 0; w < 3; ++w)
    {
        sf::Vector2i aPos(pos.x + 0.34f * (float)w * size.x, pos.y + 0.34f * (float)h * size.y);
        areas[h*3 + w] = sf::Rect<int>(aPos, sf::Vector2i(areaSize.x, areaSize.y));   
    }

    horizontalLines[0].setPosition(pos.x, pos.y + size.y * 0.32f);
    horizontalLines[0].setSize(sf::Vector2f(size.x, size.y * 0.01f));
    horizontalLines[0].setFillColor(sf::Color::Green);
    horizontalLines[1].setPosition(pos.x, pos.y + size.y * 0.66f);
    horizontalLines[1].setSize(sf::Vector2f(size.x, size.y * 0.01f));
    horizontalLines[1].setFillColor(sf::Color::Green);

    verticalLines[0].setPosition(pos.x + size.x * 0.32f, pos.y);
    verticalLines[0].setSize(sf::Vector2f(size.x * 0.01f, size.y));
    verticalLines[0].setFillColor(sf::Color::Green);
    verticalLines[1].setPosition(pos.x + size.x * 0.66f, pos.y);
    verticalLines[1].setSize(sf::Vector2f(size.x * 0.01f, size.y));
    verticalLines[1].setFillColor(sf::Color::Green);
}

BigGrid::BigGrid(const sf::Vector2f& pos, const sf::Vector2f& size)
: gridWinnerChar('-')
, visual(pos, size)
, mLastMove(Game::Position(-1, -1), true)
{
    for (int i = 0; i < 9; ++i)
    {   
        int h = i / 3;
        int w = i % 3;
        grid.push_back(SmallGrid(
            sf::Vector2f(pos.x + w * 0.34f * size.x + 0.032f * size.x,
                         pos.y + h * 0.34f * size.y + 0.032f * size.y), 
                        sf::Vector2f(size.x * 0.32f * 0.8f, size.y * 0.32f * 0.8f)));
    }
}

bool BigGrid::makeMove(const Game::Move& move)
{
    // const Position& bigPos, const Position& smallPos, char ch
    Position bigPos = getBigPosition(move.globalPos);
    Position smallPos = getSmallPosition(move.globalPos);
    char ch = getCharAccordingToTurn(move.crossesTurn);

    auto& smallGrid = grid[bigPos.y * 3 + bigPos.x];
    bool success = smallGrid.makeMove(smallPos, ch);

    if (success)
    {
        mLastMove = move;
        gridWinnerChar = getWinnerChar(); 
        updateAvailability();
        return true;
    }
    else
    {
        return false;
    }

}


void BigGrid::updateAvailability()
{

    int indx = getLocalIndxFromGlobalPos(mLastMove.globalPos);
    if (mLastMove.globalPos.x == -1 || grid[indx].isGridDone())
    {
        for (int i = 0; i < 9; ++i)
            grid[i].setAvailability(true);
    }
    else
    {
        for (int i = 0; i < 9; ++i)
            grid[i].setAvailability(false);

        grid[indx].setAvailability(true);
        std::cout << "INDX: " << indx << " IS AVAILABLE!\n";
    }
}

bool BigGrid::isGridDone() const
{
    bool full = true;
    for (int i = 0; i < 9; ++i)
    {
        if (!grid[i].isGridDone())
        {
            full = false;
            break;
        }
    }
    return (gridWinnerChar != empty || full);
}

char BigGrid::getWinner() const
{
    // debug
    // assert(gridWinnerChar != empty);

    return gridWinnerChar;
}

sf::Rect<int>* BigGrid::getVisualAreas()
{
    return visual.areas;
}

const SmallGrid& BigGrid::getSmallGrid(int smallGridIndx) const
{
    return grid[smallGridIndx];
}

sf::Rect<int>* BigGrid::getSmallGridVisualAreas(int smallGridIndx)
{
    if (smallGridIndx < 0 || smallGridIndx >= 9)
        return nullptr;

    return grid[smallGridIndx].getVisualAreas();
}

std::vector<Game::Position> BigGrid::getAvailMoves() const
{

}

Game::Move BigGrid::getLastMove() const
{
    return mLastMove;
}

char BigGrid::getWinnerChar()
{
    char charGrid[3][3];
    for (int h = 0; h < 3; ++h)
    for (int w = 0; w < 3; ++w)
    {
        charGrid[h][w] = grid[h*3 + w].isGridDone() ? grid[h*3 + w].getWinner() : empty;
    }


    // horizontal check
    for (int h = 0; h < 3; ++h)
    {
        if (charGrid[h][0] == empty) continue;

        if (charGrid[h][0] == charGrid[h][1] && charGrid[h][1] == charGrid[h][2])
            return charGrid[h][0];       
    }

    // vertical check
    for (int w = 0; w < 3; ++w)
    {
        if (charGrid[0][w] == empty) continue;

        if (charGrid[0][w] == charGrid[1][w] && charGrid[1][w] == charGrid[2][w])
            return charGrid[0][w];       
    }

    // diagonal check
    if (charGrid[1][1] != empty)
    {
        // diagonal 1
        if (charGrid[0][0] == charGrid[1][1] && charGrid[1][1] == charGrid[2][2])
            return charGrid[1][1];
        // diagonal 2
        if (charGrid[0][2] == charGrid[1][1] && charGrid[1][1] == charGrid[2][0])
            return charGrid[1][1];
    }

    return empty;
}

void BigGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{   
    for (int i = 0; i < 2; ++i)
    {
        target.draw(visual.verticalLines[i]);
        target.draw(visual.horizontalLines[i]);
    }
    for (int i = 0; i < 9; ++i)
    {
        target.draw(grid[i]);
    }
}

// //------------------------------------------------------------------------------------------
// SmallGridVisual::SmallGridVisual(const sf::Vector2f& pos, const sf::Vector2f& size)
// : gridRect(pos, size)
// {
//     const sf::Vector2f areaSize(size.x * 0.32f, size.y * 0.32f);
//     for (int h = 0; h < 3; ++h)
//     for (int w = 0; w < 3; ++w)
//     {
//         areas[h*3 + w].setPosition(pos.x + 0.34f * (float)w * size.x, pos.y + 0.34f * (float)h * size.y);
//         areas[h*3 + w].setSize(areaSize);
//     }

//     horizontalLines[0].setPosition(pos.x, pos.y + size.y * 0.32f);
//     horizontalLines[0].setSize(sf::Vector2f(size.x, size.y * 0.02f));
//     horizontalLines[1].setPosition(pos.x, pos.y + size.y * 0.66f);
//     horizontalLines[1].setSize(sf::Vector2f(size.x, size.y * 0.02f));

//     verticalLines[0].setPosition(pos.x + size.x * 0.32f, pos.y);
//     verticalLines[0].setSize(sf::Vector2f(size.x * 0.02f, size.y));
//     verticalLines[1].setPosition(pos.x + size.x * 0.66f, pos.y);
//     verticalLines[1].setSize(sf::Vector2f(size.x * 0.02f, size.y));
// }

// void SmallGridVisual::draw(sf::RenderTarget& target, sf::RenderStates states) const
// {
//     for (int i = 0; i < 9; ++i)
//         target.draw(areas[i]);
    
//     for (int i = 0; i < 2; ++i)
//     {
//         target.draw(verticalLines[i]);
//         target.draw(horizontalLines[i]);
//     }
// }

// //----------------------------------------
// BigGridVisual::BigGridVisual(const sf::Vector2f& pos, const sf::Vector2f& size)
// : gridRect(pos, size)
// {
//     const sf::Vector2f areaSize(size.x * 0.32f, size.y * 0.32f);
//     for (int h = 0; h < 3; ++h)
//     for (int w = 0; w < 3; ++w)
//     {
//         areas[h*3 + w].setPosition(pos.x + 0.34f * (float)w * size.x, pos.y + 0.34f * (float)h * size.y);
//         areas[h*3 + w].setSize(areaSize);
//     }

//     horizontalLines[0].setPosition(pos.x, pos.y + size.y * 0.32f);
//     horizontalLines[0].setSize(sf::Vector2f(size.x, size.y * 0.02f));
//     horizontalLines[1].setPosition(pos.x, pos.y + size.y * 0.66f);
//     horizontalLines[1].setSize(sf::Vector2f(size.x, size.y * 0.02f));

//     verticalLines[0].setPosition(pos.x + size.x * 0.32f, pos.y);
//     verticalLines[0].setSize(sf::Vector2f(size.x * 0.02f, size.y));
//     verticalLines[1].setPosition(pos.x + size.x * 0.66f, pos.y);
//     verticalLines[1].setSize(sf::Vector2f(size.x * 0.02f, size.y));
// }

// void BigGridVisual::draw(sf::RenderTarget& target, sf::RenderStates states) const
// {
//     for (int i = 0; i < 9; ++i)
//         target.draw(areas[i]);
    
//     for (int i = 0; i < 2; ++i)
//     {
//         target.draw(verticalLines[i]);
//         target.draw(horizontalLines[i]);
//     }
// }



}