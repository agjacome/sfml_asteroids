#include "asteroid.hpp"
#include "utils.hpp"

#include <cmath>
#include <iostream>

namespace asteroids
{

constexpr const float Asteroid::SPEED  [ ];
constexpr const float Asteroid::RADIUS [ ];

// create asteroid with initial position, direction and level
Asteroid::Asteroid(sf::Vector2f &position, float angle, sf::Uint8 level)
    : GameObject(true), m_level(level)
{
    // set direction
    const float rad = radians(angle);
    m_direction.x = std::cos(rad);
    m_direction.y = std::sin(rad);

    // create shape
    m_shape.setRadius(RADIUS[m_level]);
    m_shape.setFillColor(random_color());
    m_shape.setOutlineColor(sf::Color::White);
    m_shape.setOutlineThickness(1);
    m_shape.setOrigin(RADIUS[m_level], RADIUS[m_level]);

    // update position of transformable
    setPosition(position);
}

// update the asteroid
void Asteroid::update(float delta_time)
{
    if (!m_alive) return;

    // update position
    sf::Vector2f velocity = m_direction * SPEED[m_level] * delta_time;
    move(velocity);

    // check screen limits, and teleport asteroid accordingly
    sf::Vector2f position = move_to_limits(getPosition(), RADIUS[m_level]);
    setPosition(position);
}

// implementation of virtual draw() of sf::Drawable
void Asteroid::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // draw the shape
    states.transform *= getTransform();
    target.draw(m_shape, states);
}

// increment asteroid level (make it smaller)
void Asteroid::incr_level( )
{
    if (++m_level >= NUM_LEVELS) { kill(); return; }

    m_shape.setRadius(RADIUS[m_level]);
    m_shape.setOrigin(RADIUS[m_level], RADIUS[m_level]);
}

// change the angle of movement
void Asteroid::change_angle(float angle)
{
    const float rads = radians(angle);
    m_direction.x = std::cos(rads);
    m_direction.y = std::sin(rads);
}

// check collision between asteroid and ship (circle-triangle)
bool Asteroid::check_collision(const Ship &ship) const
{
    // there is a collision between a triangle and a circle if any of the
    // segments conforming the triangle collides with the circle
    const auto points = ship.get_points();

    return check_collision(points[0], points[1]) ||
           check_collision(points[0], points[2]) ||
           check_collision(points[1], points[2]);
}

// check collision between asteroid and bullet (circle-circle)
bool Asteroid::check_collision(const Bullet &bullet) const
{
    // c1 = asteroid center, c2 = bullet center
    // r1 = asteroud radius, r2 = bullet radius
    const sf::Vector2f c1 = getPosition();
    const sf::Vector2f c2 = bullet.getPosition();
    const float        r1 = RADIUS[m_level];
    const float        r2 = bullet.get_radius();

    // if distance between center points is less or equal than the sum of the
    // radii, then there is a collision
    const float distance = std::pow(c2.x - c1.x, 2) + std::pow(c2.y - c1.y, 2);
    const float radiisum = std::pow(r1 + r2, 2);
    return distance <= radiisum;
}

// check collision with a given segment (two points)
bool Asteroid::check_collision(const sf::Vector2f &p, const sf::Vector2f &q) const
{
    const sf::Vector2f d = q - p;               // direction vector of segment
    const sf::Vector2f f = p - getPosition();   // vector center->segment_start

    // necessary dot products a=d·d, b=2*f·d, c=f·f-r^2
    const float a = d.x * d.x + d.y * d.y;
    const float b = 2 * (f.x * d.x + f.y * d.y);
    const float c = (f.x * f.x + f.y * f.y) - std::pow(RADIUS[m_level], 2);

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0.0f) return false;      // no solution: no intersection

    // solution may be on or off the segment, so need to test both
    // t1 is always smaller than t2, because discrimnant and a are nonnegative
    discriminant = std::sqrt(discriminant);
    const float t1 = (-b - discriminant) / (2 * a);
    const float t2 = (-b + discriminant) / (2 * a);

    if (t1 >= 0.0f && t1 <= 1.0f) return true;  // t1 is an intersection
    if (t2 >= 0.0f && t2 <= 1.0f) return true;  // t2 is an intersection
    return false;                               // else, no intersection
}

}
