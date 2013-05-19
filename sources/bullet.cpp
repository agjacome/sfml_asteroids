#include "bullet.hpp"
#include "utils.hpp"

namespace asteroids
{

// bullet is created on initial position and angle (move direction)
Bullet::Bullet(sf::Vector2f &position, float angle)
    : GameObject(true), m_remaining(LIFE_TIME)
{
    // initialize direction vector to given angle
    const float rad = radians(angle);
    m_direction = sf::Vector2f(std::cos(rad), std::sin(rad));

    // create shape (circle of radius 1.0)
    m_shape.setRadius(1.0f);
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOutlineThickness(0.0f);
    m_shape.setOrigin(1.0f, 1.0f);

    // set in given position
    setPosition(position);
}

// update position and life of bullet
void Bullet::update(float delta_time)
{
    if (!m_alive) return;

    // update remaining life, and kill if necessary
    m_remaining -= delta_time;
    if (m_remaining <= 0.0f) kill();

    // update bullet position
    sf::Vector2f velocity = m_direction * VELOCITY * delta_time;
    move(velocity);

    // check screen limits, and teleport bullet accordingly
    sf::Vector2f position = move_to_limits(getPosition(), 1.0f);
    setPosition(position);
}

// implementation of virtual draw() of sf::Drawable
void Bullet::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // draw the bullet shape
    states.transform *= getTransform();
    target.draw(m_shape, states);
}

}
