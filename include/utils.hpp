#pragma once

#include "screen.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>

namespace asteroids
{

// convert to radians an angle given in degrees
constexpr float radians(float degrees)
{
    return degrees * M_PI / 180.0f;
}

// wrap-around elements in screen: left-to-right, top-to-bottom
// return a vector moved to its complementary position if it goes off limits of
// the screen + a given margin
inline sf::Vector2f move_to_limits(sf::Vector2f position, float margin)
{
    const float x_low_limit = -margin, x_high_limit = Screen::WIDTH  + margin;
    const float y_low_limit = -margin, y_high_limit = Screen::HEIGHT + margin;

    if      (position.x <= x_low_limit)  position.x = x_high_limit;
    else if (position.x >= x_high_limit) position.x = x_low_limit;
    if      (position.y <= y_low_limit)  position.y = y_high_limit;
    else if (position.y >= y_high_limit) position.y = y_low_limit;

    return position;
}

// return a random SFML color (with alpha = 0xFF always)
inline sf::Color random_color ( )
{
    static std::random_device rand_dev;
    static std::mt19937       rand_eng(rand_dev());
    static std::uniform_int_distribution<> rand_color(0, 255);

    const sf::Uint8 red = rand_color(rand_eng);
    const sf::Uint8 gre = rand_color(rand_eng);
    const sf::Uint8 blu = rand_color(rand_eng);

    return sf::Color(red, gre, blu, 255);
}

}
