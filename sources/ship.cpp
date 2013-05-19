#include "ship.hpp"
#include "screen.hpp"
#include "utils.hpp"

namespace asteroids
{

// ship constructor
Ship::Ship( )
    : GameObject(true), m_velocity(0.0f, 0.0f)
{
    // ship shape definition (triangle)
    m_shape.setPointCount(3);
    m_shape.setPoint(0, sf::Vector2f( 10.0f,  0.0f));
    m_shape.setPoint(1, sf::Vector2f(-10.0f,  7.5f));
    m_shape.setPoint(2, sf::Vector2f(-10.0f, -7.5f));

    // ship shape appearance
    m_shape.setFillColor(sf::Color(200, 200, 200, 255));
    m_shape.setOutlineColor(sf::Color::White);
    m_shape.setOutlineThickness(2);
    m_shape.setPosition(0.0f, 0.0f);

    // default values
    reset();
}

// reset ship to default values
void Ship::reset( )
{
    setRotation(0.0f);                                  // zero rotation
    m_velocity.x = m_velocity.y = 0.0f;                 // stopped
    setPosition(Screen::WIDTH / 2, Screen::HEIGHT / 2); // screen centered
}

// update position, rotation and internal logic
void Ship::update(float delta_time)
{
    // rotate ship clock or counterclockwise
    float rotation = ROTATION_VEL * delta_time;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  rotate(-rotation);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) rotate(rotation);

    // increment or decrement ship velocity (acceleration/deceleration)
    rotation = getRotation();
    float vel_x = ACCELERATION * delta_time * std::cos(radians(rotation));
    float vel_y = ACCELERATION * delta_time * std::sin(radians(rotation));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        m_velocity.x += vel_x, m_velocity.y += vel_y;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        m_velocity.x -= vel_x, m_velocity.y -= vel_y;

    // if ship is moving, we apply friction and set new position
    float speed = std::sqrt(std::pow(m_velocity.x,2) + std::pow(m_velocity.y,2));
    if (speed == 0.0f) return;

    float angle = std::atan2(m_velocity.y / speed, m_velocity.x / speed);
    m_velocity.x -= DRAG_FORCE * delta_time * std::cos(angle);
    m_velocity.y -= DRAG_FORCE * delta_time * std::sin(angle);

    // stop ship if velocity < 0, don't allow it to go backwards
    speed = std::sqrt(std::pow(m_velocity.x, 2) + std::pow(m_velocity.y, 2));
    if (speed < 0.0f) m_velocity.x = m_velocity.y = 0.0f;

    // update ship position
    move(m_velocity * delta_time);

    // check screen limits, and teleport ship accordingly
    sf::Vector2f position = move_to_limits(getPosition(), 10.0f);
    setPosition(position);
}

// implementation of virtual draw() of sf::Drawable
void Ship::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // apply ship transformation and draw it
    states.transform *= getTransform();
    target.draw(m_shape, states);
}

// return a vector of all ship shape points
std::vector<sf::Vector2f> Ship::get_points( ) const
{
    std::vector<sf::Vector2f> points;
    points.push_back(getTransform().transformPoint(m_shape.getPoint(0)));
    points.push_back(getTransform().transformPoint(m_shape.getPoint(1)));
    points.push_back(getTransform().transformPoint(m_shape.getPoint(2)));

    return points;
}

}
