#include "StateStack.hpp"

StateStack::PendingChange::PendingChange(Action action,
        States::ID stateID)
: action(action)
, stateID(stateID)
{
}

StateStack::StateStack(State::Context context)
: mContext(context)
{

}

void StateStack::update()
{
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->update())
        {
            break;
        }
    }
    applyPendingChanges();
}

void StateStack::draw() const
{
    for (auto& state : mStack)
    {
        state->draw();
    }
}

void StateStack::handleEvent(const sf::Event& event)
{
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->handleEvent(event))
        {
            break;
        }
    }
    applyPendingChanges();
}

void StateStack::applyPendingChanges()
{
    for (auto& pChange : mPendingChanges)
    {
        if (pChange.action == Action::Push)
        {
            mStack.push_back(mStateFactory[pChange.stateID]());
        }
        if (pChange.action == Action::Pop)
        {       
            mStack.pop_back();
        }
        if (pChange.action == Action::Clear)
        {
            mStack.clear();
        }
    }
    mPendingChanges.clear();
}

void StateStack::pushState(States::ID stateID)
{
    mPendingChanges.push_back(PendingChange(Action::Push, stateID));
}

void StateStack::popState()
{
    mPendingChanges.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStates()
{
    mPendingChanges.push_back(PendingChange(Action::Clear));
}

bool StateStack::isEmpty() const
{
    return mStack.empty();
}