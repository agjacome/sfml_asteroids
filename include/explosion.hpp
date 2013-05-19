#pragma once

#include "gameobject.hpp"
#include "utils.hpp"

#include <SFML/Graphics.hpp>
#include <array>

namespace asteroids
{

// particles for the explosion, each one has a position and a velocity vector
struct Particle
{
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
};

class Explosion : public GameObject
{

public:
    // create a explosion in a given position and with a given life time
    Explosion (const sf::Vector2f &position, float max_life);

    // implement GameObjects pure virtual methods
    void update (float delta_time);
    void draw   (sf::RenderTarget &target, sf::RenderStates states) const;

    // static constants
    constexpr static const float MAX_SPEED     = 140.0f;
    constexpr static const float ANGLE_RAD     = radians(30.0f);
    constexpr static const int   NUM_PARTICLES = 20;

private:
    float       m_cur_life;     // life time consumed until now
    const float m_max_life;     // maximum given time (given in constructor)
    std::array<Particle, NUM_PARTICLES> m_explosion; // the explosion particles

};

}
