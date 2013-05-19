#include "explosion.hpp"

#include <cmath>

namespace asteroids
{

// create a explosion in a given position and with a given life time
Explosion::Explosion(const sf::Vector2f &position, float max_life)
    : GameObject(true), m_cur_life(0.0f), m_max_life(max_life)
{
    // assign a random velocity and the given initial position to all the
    // explosion particles
    for (unsigned int i = 0; i < NUM_PARTICLES; ++i) {
        const float velx = std::cos((i + 1) * ANGLE_RAD) * MAX_SPEED;
        const float vely = std::sin((i + 1) * ANGLE_RAD) * MAX_SPEED;

        m_explosion[i].m_position   = sf::Vector2f(position);
        m_explosion[i].m_velocity.x = velx * (std::rand() * 1.0f / RAND_MAX);
        m_explosion[i].m_velocity.y = vely * (std::rand() * 1.0f / RAND_MAX);
    }
}

// update position and life of particle
void Explosion::update(float delta_time)
{
    for (Particle &p : m_explosion) {
        p.m_position.x += p.m_velocity.x * delta_time;
        p.m_position.y += p.m_velocity.y * delta_time;
    }

    m_cur_life += delta_time;
    if (m_cur_life >= m_max_life) kill();
}

// implementation of virtual draw() of sf::Drawable
void Explosion::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // draw all the particles in the screen as SFML points (vetices), with
    // random color yellow or red
    std::array<sf::Vertex, NUM_PARTICLES> points;

    for (unsigned int i = 0; i < NUM_PARTICLES; ++i) {
        points[i].position = m_explosion[i].m_position;
        points[i].color = std::rand() % 2 ? sf::Color::Red : sf::Color::Yellow;
    }

    target.draw(points.data(), NUM_PARTICLES, sf::Points);
}

}
