#ifndef GAMEUTILITY_HPP
#define GAMEUTILITY_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <array>
#include <vector>

namespace Game
{

const char cross = 'x';
const char naught = '0';
const char empty = '-';

struct Position
{
    Position(int x = 0, int y = 0);
    int x;
    int y;

    bool operator==(const Position& pos);
};

struct Move
{
    Move(const Position& pos, bool crossesTurn);


    Position    globalPos;
    bool        crossesTurn;
};

Game::Position  getBigPosition(const Game::Position& globalPos);
Game::Position  getSmallPosition(const Game::Position& globalPos);
Game::Position  fromSmallToGlobal(const Game::Position smallPos, int bigIndx);
int             getGlobalIndxFromGlobalPos(const Game::Position& globalPos);
int             getLocalIndxFromGlobalPos(const Game::Position& globalPos);
char            getCharAccordingToTurn(bool crossesTurn);

class SmallGrid : public sf::Drawable, public sf::Transformable
{
    public:
        struct Visual
        {
            Visual(const sf::Vector2f& pos, const sf::Vector2f& size);

            sf::Rect<float>     gridRect;

            sf::RectangleShape  horizontalLines[2];
            sf::RectangleShape  verticalLines[2];
            sf::Rect<int>       areas[9];
        };

    public:
        SmallGrid(const sf::Vector2f& pos, const sf::Vector2f& size);

        bool    makeMove(const Position& smallPos, char ch);

        bool    isGridDone() const;
        char    getWinner() const;
        void    setAvailability(bool isAvailable);

        // crutch?
        std::vector<Game::Position> getLocalAvailMoves() const;
        sf::Rect<int>*              getVisualAreas();
    
    private:
        char    getWinnerChar();
        void    draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        char    grid[3][3];
        char    gridWinnerChar;

        Visual  visual;
        bool    availableToPlay;
};

class BigGrid : public sf::Drawable, public sf::Transformable
{
    public:
        struct Visual
        {
            Visual(const sf::Vector2f& pos, const sf::Vector2f& size);

            sf::Rect<float>     gridRect;

            sf::RectangleShape  horizontalLines[2];
            sf::RectangleShape  verticalLines[2];
            sf::Rect<int>       areas[9];
        };

    public:
        BigGrid(const sf::Vector2f& pos, const sf::Vector2f& size);

        bool        isGameEnd();
        bool        makeMove(const Game::Move& move);

        bool        isGridDone() const;
        char        getWinner() const;

        sf::Rect<int>*      getVisualAreas();
        sf::Rect<int>*      getSmallGridVisualAreas(int smallGridIndx);

        const SmallGrid&                getSmallGrid(int smallGridIndx) const;
        std::vector<Game::Position>     getAvailMoves() const;
        Game::Move                      getLastMove() const;
    
    private:
        void    updateAvailability();
        char    getWinnerChar();
        void    draw(sf::RenderTarget& target, sf::RenderStates states) const;
    

    private:
        std::vector<SmallGrid>  grid;
        char                    gridWinnerChar;

        Visual                  visual;
        Game::Move              mLastMove;
};

// //------------------------------------------------------------------------------

// class SmallGridVisual : public sf::Drawable, public sf::Transformable
// {
//     public:
//         SmallGridVisual(const sf::Vector2f& pos, const sf::Vector2f& size);

    // private:
    //     void    draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
//     private:
//         sf::Rect<float>     gridRect;

//         sf::RectangleShape  horizontalLines[2];
//         sf::RectangleShape  verticalLines[2];
//         sf::RectangleShape  areas[9];
// };


// class BigGridVisual : public sf::Drawable, public sf::Transformable
// {
//     public:
//         BigGridVisual(const sf::Vector2f& pos, const sf::Vector2f& size);

//     private:
//         void    draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
//     private:
//         sf::Rect<float>     gridRect;

//         sf::RectangleShape  horizontalLines[2];
//         sf::RectangleShape  verticalLines[2];
//         sf::RectangleShape  areas[9];
// };




}

#endif