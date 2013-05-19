#pragma once

#include "gameobject.hpp"
#include "bullet.hpp"
#include "ship.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

namespace asteroids
{

class Asteroid : public GameObject
{

public:
    // create asteroid with given position, angle and level
    Asteroid (sf::Vector2f &position, float angle, sf::Uint8 level = 0);

    // implement GameObjects pure virtual methods
    void update (float delta_time);
    void draw   (sf::RenderTarget &target, sf::RenderStates states) const;

    // increment current astreoid level
    void incr_level ( );

    // return current level
    sf::Uint8 get_level ( ) const { return m_level; }

    // modify direction of asteroid given a new angle
    void change_angle (float angle);

    // collision checkig between this and Ship or Bullet
    bool check_collision (const Ship &ship) const;
    bool check_collision (const Bullet &bullet) const;

    // static constants
    constexpr static const int   NUM_LEVELS = 3;
    constexpr static const float SPEED[ ]   = { 40.0f, 60.0f, 80.0f };
    constexpr static const float RADIUS[ ]  = { 30.0f, 15.0f, 7.0f  };

private:
    // collision checking with a given segment ab
    bool check_collision (const sf::Vector2f &p, const sf::Vector2f &q) const;

    sf::Uint8       m_level;     // level of asteroid
    sf::Vector2f    m_direction; // movement direction vector
    sf::CircleShape m_shape;     // graphic representation

};

}
