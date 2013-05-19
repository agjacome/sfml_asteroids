#include "game.hpp"
#include <climits>

using namespace asteroids;

int main (void)
{
    unsigned int level = 1;         // start in level 1
    unsigned int lifes = 3;         // start with 3 lifes

    // while level does not overflow, keep on playing, incrementing level each
    // time one is passed
    while (level < UINT_MAX) {
        Game sfml_asteroids(level, lifes);
        sfml_asteroids.loop();

        // if game over (real game over or user requesting exit), stop playing
        if (sfml_asteroids.game_over()) break;

        level++;
        lifes = sfml_asteroids.lifes();
    }

    return 0;
}
