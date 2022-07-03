#ifndef BOARD_HPP
#define BOARD_HPP

#include "GameUtility.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <array>
#include <vector>

namespace Game
{


class Board : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
    public:
        Board(const sf::Vector2u& mWindowSize);

        void    draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void    update();

        bool    processMove(const Game::Move& move);
        void    takebackMove();
        bool    isGameOver() const;
        char    getWinnerChar() const;

        // maybe crutch
        std::vector<Game::Position>         getAvailMoves() const;
        const std::vector<Game::Move>&      getMovesDone() const;

        Game::Position  getGlobalPosFromMouse(const sf::Vector2i& mPos);
        Game::Position  getLastMovePos() const;        
    
    private:
        // BoardLogic                      mBoardLogic;
        sf::Rect<int>                   mRect;
        BigGrid                         mGrid;
        sf::Vector2u                    mWindowSize;
        std::vector<Game::Move>         mMoves;

        
};


}

#endif