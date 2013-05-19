#pragma once

#include "gameobject.hpp"
#include <SFML/Graphics.hpp>

namespace asteroids
{

class Screen
{

public:
    // create a empty screen with defined constant window dimensions
    Screen ( ) : m_window(sf::VideoMode(WIDTH, HEIGHT), "Asteroids") {  }

    // wrapper methods
    void clear ( ) { m_window.clear();   }
    void close ( ) { m_window.close();   }
    void show  ( ) { m_window.display(); }

    // draws GameObjects or SFML's Text in the screen
    void draw (const sf::Text &txt)   { m_window.draw(txt); }
    void draw (const GameObject &obj) { m_window.draw(obj); }

    // return in &event the event in top of the queue, and a bool value
    // indicating if there was any (true) or not (false)
    bool event (sf::Event &event) { return m_window.pollEvent(event); }

    // static constants, window dimensions
    constexpr static const int WIDTH  = 640;
    constexpr static const int HEIGHT = 480;

private:
    sf::RenderWindow m_window;  // the window to manipulate

};

}
