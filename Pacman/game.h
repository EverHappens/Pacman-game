#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H

#include "pacman.h"

struct Textures{
    sf::Texture pellets;
    sf::Texture sprites;
    sf::Texture map;
    sf::Texture mapWhite;
    sf::Texture font;
};
extern Textures textures;
void Start();

void MainLoop();

void EndGame();

void GameLoop();
#endif //PACMAN_GAME_H
