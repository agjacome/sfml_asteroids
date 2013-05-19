#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace asteroids
{

// GameObject abstract class, works as a common interface for all game objects
class GameObject : public sf::Transformable, public sf::Drawable
{

public:
    virtual ~GameObject ( ) { }

    // update the object position, rotation and all its internal logic
    virtual void update (float) = 0;

    // sf::Drawable::draw(), draws the object in a window
    virtual void draw (sf::RenderTarget &, sf::RenderStates) const = 0;

    // returns if the object is currentyl alive
    virtual bool alive ( ) const { return m_alive; }

    // kills the object (set it to not alive)
    virtual void kill ( ) { m_alive = false; }

protected:
    GameObject (bool alive) : m_alive(alive) { }

    bool m_alive;   // represents if the object is alive or dead

};

}
