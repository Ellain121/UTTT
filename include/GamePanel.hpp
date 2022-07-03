#ifndef GAMEPANEL_HPP
#define GAMEPANEL_HPP

namespace Game
{
class Board;
}
class InputManager;

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class GamePanel : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
    public:
        GamePanel(Game::Board* board, InputManager* inpManager);

        void        processPossibleClicks();
        bool        contains(sf::Vector2i pos) const;

    private:
        void        draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
    private:
        Game::Board&        mBoard;
        InputManager&       mInputManager;
};



#endif