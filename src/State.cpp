#include "State.hpp"
#include "StateStack.hpp"

State::Context::Context(sf::RenderWindow& window, const TextureHolder& textureHolder,
                                        const FontHolder& fontHolder)
: window(&window)
, textures(&textureHolder)
, fonts(&fontHolder)
{
}

State::State(StateStack& stack, Context context)
: mStateStack(&stack)
, mContext(context)
{

}

void State::requestStackPush(States::ID stateID)
{
    mStateStack->pushState(stateID);
}

void State::requestStackPop()
{
    mStateStack->popState();
}

void State::requestStackClear()
{
    mStateStack->clearStates();
}

State::Context State::getContext() const
{
    return mContext;
}