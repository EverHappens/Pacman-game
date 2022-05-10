#ifndef PACMAN_PACMAN_H
#define PACMAN_PACMAN_H

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include "global.hpp"
#include "mapCollision.h"
#include "cell.h"
#include "game.h"

enum Direction {
    up,
    down,
    left,
    right,
    none
};

enum GhostType {
    blinky,
    pinky,
    inky,
    clyde
};

enum BehaviorType {
    chase,
    scatter,
    frightened,
};

enum State {
    play,
    start,
    lose,
    over,
    win,
    close
};

class Ghost {
    sf::Vector2f pos = {0, 0};
    Direction dir = none;
    BehaviorType behaviorType = chase;
    GhostType ghostType;
    float movementSpeed = MOVEMENT_SPEED;
    bool useHome = true;
public:
    Ghost();

    Ghost(unsigned int x, unsigned int y, GhostType, bool canUseHome = true);

    GhostType getType();

    void makeScatter();

    void unscatter();
    sf::Vector2f getPos();

    sf::Vector2f getChaseTarget();

    sf::Vector2f getScatterTarget();

    sf::Vector2f getFrightenedTarget();

    Direction bestDir(sf::Vector2f target);

    void update();
};

class Pacman {
    sf::Vector2f pos = {10 * CELL_SIZE, 15 * CELL_SIZE};
    Direction dir = none;
public:
    Pacman();

    Pacman(unsigned int x, unsigned int y);

    void update();

    bool isStuck();

    sf::Vector2f getPos();

    Direction getDir();
};

struct GameState {
    Pacman *pacman;
    State state = start;
    int pauseTime = 20000;
    std::vector<Ghost *> ghosts;
    sf::RenderWindow *window;
    std::array<std::string, MAP_WIDTH> currentMap;
    unsigned pelletCount;
    int energizerTime = 0;
    int lives = 3;
};

extern GameState gameState;
#endif //PACMAN_PACMAN_H
