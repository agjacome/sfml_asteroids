#include "game.hpp"
#include "asteroid.hpp"
#include "explosion.hpp"
#include "bullet.hpp"
#include "ship.hpp"
#include "utils.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <algorithm>
#include <cmath>
#include <string>

namespace asteroids
{

constexpr const char Game::FONT_PATH [ ];

// create a game in given level and with given restant lifes
Game::Game(unsigned int level, unsigned int lifes)
    : m_running(false), m_gameover(false), m_launch(0.0f), m_lifes(lifes)
{
    // create ship and all starting asteroids
    m_objects.push_back(std::make_shared<Ship>());

    sf::Vector2f position;
    for (unsigned int i = 0; i < START_ASTEROIDS + level; ++i) {
        auto angle = std::rand() % 360;
        position.x = std::rand() % Screen::WIDTH;
        position.y = std::rand() % Screen::HEIGHT;
        m_objects.push_back(std::make_shared<Asteroid>(position, angle));
    }

    // load font and create text displays
    m_font.loadFromFile(FONT_PATH);
    m_level_txt.setString(std::string("Level: " + std::to_string(level)));
    m_level_txt.setFont(m_font);
    m_level_txt.setCharacterSize(15);
    m_level_txt.setColor(sf::Color::White);
    m_level_txt.setPosition(10.0f, 5.0f);
    m_lifes_txt.setString(std::string("Lifes: " + std::to_string(m_lifes)));
    m_lifes_txt.setFont(m_font);
    m_lifes_txt.setCharacterSize(15);
    m_lifes_txt.setColor(sf::Color::White);
    m_lifes_txt.setPosition(10.0f, 25.0f);
}

// game loop
void Game::loop( )
{
    float frame_time  = 0.0f;
    float accumulator = 0.0f;

    sf::Clock timer;

    m_running = true;
    while (m_running) {
        frame_time   = timer.restart().asSeconds();
        accumulator += frame_time;

        while (accumulator >= UPDATE_FREQ) {
            handle_events();
            handle_collisions();
            update_objects();
            accumulator -= UPDATE_FREQ;
        }

        render_objects();
    }

    m_screen.close();
}

// handle possible screen/keyboard events
void Game::handle_events( )
{
    // if spacebar is pressed, launch a bullet
    // if the window is closed or escape is pressed, exit game

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        launch_bullet();

    sf::Event event;
    while (m_screen.event(event)) {
        bool exit = event.type == sf::Event::Closed ||
                    event.key.code == sf::Keyboard::Escape;
        if (exit) { m_running = false; m_gameover = true; return; }
    }
}

// launcha new bullet
void Game::launch_bullet( )
{
    // if not enough time has passed, do nothing
    if (m_launch > 0.0f) return;

    // get current angle and position of spaceship
    const std::shared_ptr<GameObject> SHIP = m_objects.front();
    sf::Vector2f position = SHIP->getPosition();
    const float degs = SHIP->getRotation();

    // set bullet launch position
    const float rads = radians(degs);
    position += 10.0f * sf::Vector2f(std::cos(rads), std::sin(rads));

    // create the bullet
    m_objects.push_back(std::make_shared<Bullet>(position, degs));
    m_launch = Bullet::SHOOT_FREQ;
}

// draw all objects in the screen
void Game::render_objects( )
{
    m_screen.clear();

    for (auto &o : m_objects) m_screen.draw(*o);
    m_screen.draw(m_level_txt);
    m_screen.draw(m_lifes_txt);

    m_screen.show();
}

// update all objects
void Game::update_objects( )
{
    // update launch time
    m_launch -= UPDATE_FREQ;

    // delete dead (not alive) objects
    m_objects.erase(
        std::remove_if(m_objects.begin(), m_objects.end(),
            [ ] (const std::shared_ptr<GameObject> &o) { return !o->alive(); }),
        m_objects.end());

    // and update the restant (alive) objects
    for (auto &o : m_objects) o->update(UPDATE_FREQ);
}

// check if there are any collision, and procede accordingly
void Game::handle_collisions( )
{
    int count_asteroids = 0;
    std::shared_ptr<Asteroid> asteroid;

    // for each asteroid, check if it collids with any bullet or with the ship
    for (auto &a : m_objects) {
        asteroid = std::dynamic_pointer_cast<Asteroid>(a);
        if (!asteroid || !asteroid->alive()) continue;

        count_asteroids++;
        asteroid_bullet_collision(asteroid);
        asteroid_ship_collision(asteroid);
    }

    // if there are no more asteroids, then finish this game
    if (count_asteroids == 0) m_running = false;
}

// check if a given asteroid collides with any alive bullet
void Game::asteroid_bullet_collision(std::shared_ptr<Asteroid> &asteroid)
{
    std::shared_ptr<Bullet> bullet;

    // search for a colliding bullet, if not such bullet found, there is
    // nothing to do here
    auto found = std::find_if(m_objects.begin(), m_objects.end(),
            [asteroid, &bullet] (std::shared_ptr<GameObject> o)
            {
                bullet = std::dynamic_pointer_cast<Bullet>(o);
                return bullet && bullet->alive() &&
                       asteroid->check_collision(*bullet);
            });
    if (found == m_objects.end()) return;

    // if a bullet is found, kill it, increment the asteroid level and change
    // its angle (direction vector)
    bullet->kill();
    asteroid->incr_level();
    asteroid->change_angle(std::rand() % 360);

    // if the asteroid wasn't at highest level, then it will be alive after
    // incrementing its level, and it should be divided in two
    if (asteroid->alive()) {
        auto ang = std::rand() % 360;
        auto lvl = asteroid->get_level();
        auto pos = asteroid->getPosition();
        auto div = std::make_shared<Asteroid>(pos, ang, lvl);
        m_objects.push_back(div);
    }

    // create explosion associated with this collision
    auto explosion = std::make_shared<Explosion>(asteroid->getPosition(), 0.75f);
    m_objects.push_back(explosion);
}

// check if given asteroid collides with the spaceship
void Game::asteroid_ship_collision(std::shared_ptr<Asteroid> &asteroid)
{
    // if the asteroid is not alive or it does not collide with the ship, then
    // there is nothing to do here
    auto ship = std::static_pointer_cast<Ship>(m_objects.front());
    if (!asteroid->alive() || !asteroid->check_collision(*ship)) return;

    // else, decrement lifes and, if they get to zero, then is a game over
    if (--m_lifes <= 0) { m_running  = false; m_gameover = true; return; }

    // update lifes text display and reset ship to its start position
    m_lifes_txt.setString(std::string("Lifes: " + std::to_string(m_lifes)));
    ship->reset();

    // create explosion associated with this collision
    auto explosion = std::make_shared<Explosion>(asteroid->getPosition(), 0.75f);
    m_objects.push_back(explosion);
}

}
