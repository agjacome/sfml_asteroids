#pragma once

#include "asteroid.hpp"
#include "gameobject.hpp"
#include "screen.hpp"

#include <SFML/Graphics.hpp>
#include <list>
#include <memory>

namespace asteroids
{

class Game
{

public:
    // create a game in given level and with given restant lifes
    Game (unsigned int level, unsigned int lifes);

    // game loop
    void loop ( );

    // return if game is finished and number of restant lifes
    bool game_over     ( ) { return m_gameover; }
    unsigned int lifes ( ) { return m_lifes; }

    // static constants
    constexpr static const unsigned int START_ASTEROIDS = 5;
    constexpr static const float        UPDATE_FREQ     = 1.0f / 60.0f;
    constexpr static const char         FONT_PATH [ ]   = "dejavusansmono.ttf";

private:
    // private helper methods
    void handle_events     ( );
    void handle_collisions ( );
    void render_objects    ( );
    void update_objects    ( );
    void launch_bullet     ( );
    void asteroid_bullet_collision (std::shared_ptr<Asteroid> &asteroid);
    void asteroid_ship_collision   (std::shared_ptr<Asteroid> &asteroid);

    bool            m_running;      // is the game still running?
    bool            m_gameover;     // is the game finished?
    float           m_launch;       // wait time to next bullet launch
    unsigned int    m_lifes;        // current number of lifes

    Screen          m_screen;       // the screen where everythin happens
    sf::Font        m_font;         // font for stat displays
    sf::Text        m_level_txt;    // current level text display
    sf::Text        m_lifes_txt;    // current lifes text display

    std::list<std::shared_ptr<GameObject>> m_objects; // all of game objects

};

}
