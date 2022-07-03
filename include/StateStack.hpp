#ifndef STATESTACK_HPP
#define STATESTACK_HPP

#include "StateIdentifiers.hpp"
#include "State.hpp"

#include <SFML/System/NonCopyable.hpp>

#include <vector>
#include <map>
#include <functional>

class StateStack : private sf::NonCopyable
{
    public:
        enum Action
        {
            Push,
            Pop,
            Clear
        };

        struct PendingChange
        {
            PendingChange(Action action, 
                States::ID stateID = States::ID::None);
            Action      action;
            States::ID  stateID;
        };

    public:
                    StateStack(State::Context context);

        void        update();
        void        draw() const;
        void        handleEvent(const sf::Event& event);

        template<typename T>
        void        registerState(States::ID stateID);

        void        pushState(States::ID stateID);
        void        popState();
        void        clearStates();
        bool        isEmpty() const;
    
    private:
        void        applyPendingChanges();
    
    private:
        std::vector<State*>         mStack;
        std::vector<PendingChange>  mPendingChanges;
        State::Context              mContext;

        std::map<States::ID, std::function<State*()>>   mStateFactory;
};

template<typename T>
void StateStack::registerState(States::ID stateID)
{
    mStateFactory[stateID] = [&](){
        return new T(*this, mContext);
    };
}

#endif