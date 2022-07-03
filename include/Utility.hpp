#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Graphics/Rect.hpp>

template <typename SF_Object>
void centerOrigin(SF_Object& sfObject)
{
    sf::FloatRect bounds = sfObject.getLocalBounds();
    sfObject.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}


#endif
