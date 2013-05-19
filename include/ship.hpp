#pragma once

#include "gameobject.hpp"
#include <vector>

namespace asteroids
{

class Ship : public GameObject
{

public:
    Ship ( );

    // reset ship to its default values
    void reset ( );

    // implements GameObjects pure virtual methods
    void update (float delta_time);
    void draw   (sf::RenderTarget &target, sf::RenderStates states) const;

    // return a vector of all ship shape points
    std::vector<sf::Vector2f> get_points ( ) const;

    // static constants
    constexpr static const float DRAG_FORCE   = 10.0f;
    constexpr static const float ACCELERATION = 70.0f;
    constexpr static const float ROTATION_VEL = 180.0f;

private:
    sf::Vector2f    m_velocity;     // ship velocity vector
    sf::ConvexShape m_shape;        // ship geometric shape

};

}
