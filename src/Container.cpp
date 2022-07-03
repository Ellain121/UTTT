#include "Container.hpp"


Container::Container(State::Context context)
: mSelected(-1)
{

}

void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{   
    states.transform *= getTransform();
    for (const Component::Ptr& element : mComponents)
    {
        target.draw(*element, states);
    }
}

void Container::update()
{

}

bool Container::hasSelection() const
{
    return mSelected != -1;
}

void Container::select(std::size_t index)
{
    if (mComponents[index]->isSelectable())
    {
        if (hasSelection())
        {
            mComponents[mSelected]->deselect();
        }
        mComponents[index]->select();
        mSelected = index;
    }
}

void Container::selectNext()
{
    if (!hasSelection()) return;

    mComponents[mSelected]->deselect();

    do
    {
        mSelected = (mSelected + 1) % mComponents.size();
    } while (!mComponents[mSelected]->isSelectable());

    mComponents[mSelected]->select();
}

void Container::selectPrevious()
{
    if (!hasSelection()) return;

    mComponents[mSelected]->deselect();

    do
    {
        mSelected = (mSelected - 1) < 0 ? mComponents.size() - 1 : mSelected - 1;
    } while (!mComponents[mSelected]->isSelectable());

    mComponents[mSelected]->select();
}

void Container::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
        {
            selectPrevious();
        }
        if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
        {
            selectNext();
        }
        if (event.key.code == sf::Keyboard::Return)
        {
            if (hasSelection())
            {
                mComponents[mSelected]->activate();
            }
        }
    }
}

void Container::pack(Component::Ptr component)
{
    mComponents.push_back(std::move(component));

    if (!hasSelection() && mComponents[mComponents.size() - 1]->isSelectable())
    {
        select(mComponents.size() - 1);
    }
}