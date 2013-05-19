#pragma once

#include "gameobject.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace asteroids
{

class Bullet : public GameObject
{

public:
    // create a bullet in a given position and with a given angle
    Bullet (sf::Vector2f &position, float angle);

    // implement GameObject pure virtual methods
    void update (float delta_time);
    void draw   (sf::RenderTarget &target, sf::RenderStates states) const;

    // return radius of bullet
    float get_radius ( ) const { return m_shape.getRadius(); }

    // static constants
    constexpr static const float VELOCITY   = 100.0f;
    constexpr static const float LIFE_TIME  = 2.0f;
    constexpr static const float SHOOT_FREQ = 0.15f;

private:
   float           m_remaining;         // remaining life of bullet
   sf::Vector2f    m_direction;         // direction vector of bullet
   sf::CircleShape m_shape;             // shape of the bullet (circle)

};

}
