#include "GamePanel.hpp"
#include "Board.hpp"

GamePanel::GamePanel(Game::Board* board, InputManager* inpManager)
: mBoard(*board)
, mInputManager(*inpManager)
{
    
}

void GamePanel::processPossibleClicks()
{

}   

void GamePanel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}