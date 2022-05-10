#include <vector>
#include "game.h"
#include <iostream>

GameState gameState;
Textures textures;
void drawPacman() {
    if (gameState.state == play) {
        sf::CircleShape shape;
        shape.setRadius(40.f);
        sf::Vector2f pacmanPos = gameState.pacman->getPos();
        shape.setPosition(pacmanPos.x, pacmanPos.y + Y_OFFSET);
        shape.setFillColor(sf::Color::Yellow);
        (*(gameState.window)).draw(shape);
    } else if (gameState.state == lose) {

    }
}

void drawGhosts() {
    for (Ghost *ghost: gameState.ghosts) {
        sf::RectangleShape shape;
        shape.setSize({CELL_SIZE, CELL_SIZE});
        switch (ghost->getType()) {
            case GhostType::blinky:
                shape.setFillColor(sf::Color::Red);
                break;
            case GhostType::pinky:
                shape.setFillColor(sf::Color::Magenta);//it is pink
                break;
            case GhostType::inky:
                shape.setFillColor(sf::Color::Cyan);
                break;
            case GhostType::clyde:
                shape.setFillColor(sf::Color::Yellow);
                break;
        }
        sf::Vector2f ghostPos = ghost->getPos();
        shape.setPosition(ghostPos.x, ghostPos.y + Y_OFFSET);
        shape.setSize({CELL_SIZE, CELL_SIZE});

        (*(gameState.window)).draw(shape);
    }
}

void drawMap() {
    gameState.pelletCount = 0;
    for (int i = 0; i < gameState.currentMap.size(); ++i) {
        for (int j = 0; j < gameState.currentMap[i].size(); ++j) {
            if (gameState.currentMap[i][j] == '#') {
                sf::RectangleShape wall;
                wall.setSize({CELL_SIZE, CELL_SIZE});
                wall.setPosition(j * CELL_SIZE, i * CELL_SIZE + Y_OFFSET);
                wall.setFillColor(sf::Color::Blue);
                (*(gameState.window)).draw(wall);
            }
            if (gameState.currentMap[i][j] == '.') {
                ++gameState.pelletCount;
                sf::RectangleShape pellet;
                pellet.setSize({PELLET_SIZE, PELLET_SIZE});
                pellet.setPosition(j * CELL_SIZE + CELL_SIZE / 2 - PELLET_SIZE / 2,
                                   i * CELL_SIZE + CELL_SIZE / 2 - PELLET_SIZE / 2 + Y_OFFSET);
                pellet.setFillColor(sf::Color::White);
                (*(gameState.window)).draw(pellet);
            }
            if (gameState.currentMap[i][j] == 'o') {
                ++gameState.pelletCount;
                sf::RectangleShape energizer;
                energizer.setSize({PELLET_SIZE * 1.5, PELLET_SIZE * 1.5});
                energizer.setPosition(j * CELL_SIZE + CELL_SIZE / 2 - PELLET_SIZE / 2,
                                      i * CELL_SIZE + CELL_SIZE / 2 - PELLET_SIZE / 2 + Y_OFFSET);
                energizer.setFillColor(sf::Color::White);
                (*(gameState.window)).draw(energizer);
            }
            if (gameState.currentMap[i][j] == '=') {
                sf::RectangleShape door;
                door.setSize({CELL_SIZE, CELL_SIZE / 8});
                door.setPosition(j * CELL_SIZE, i * CELL_SIZE + Y_OFFSET);
                door.setFillColor(sf::Color::White);
                (*(gameState.window)).draw(door);
            }
        }
    }
}

void drawText(std::string string, float x, int y, sf::Color color) {
    for (int i = 0; i < string.length(); ++i) {
        sf::FloatRect letter = {x * CELL_SIZE, 0, 16, 16};
        letter.left = (toupper(string[i])) - ' ' * 16 + 1;
    }
}

void drawStart() {
    //drawtext with imported sprite font
}

void drawOver(){
    //возможно можно будет щзаменить шаблоннйой функций
}

void updateFrame() {
    drawPacman();
    drawGhosts();
    drawMap();

}

void SetMap() {
    gameState.currentMap = map;
}

void SetObjects() {
    gameState.ghosts = std::vector<Ghost *>(4);
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            switch (map[i][j]) {
                case '0':
                    gameState.ghosts[0] = new Ghost(j, i, GhostType::blinky, false);
                    std::cout << j << " " << i;

                    break;
                case '1':
                    gameState.ghosts[1] = new Ghost(j, i, GhostType::pinky);
                    std::cout << j << " " << i;

                    break;
                case '2':
                    gameState.ghosts[2] = new Ghost(j, i, GhostType::inky);
                    std::cout << j << " " << i;

                    break;
                case '3':
                    gameState.ghosts[3] = new Ghost(j, i, GhostType::clyde);
                    std::cout << j << " " << i;

                    break;
                case 'P':
                    gameState.pacman = new Pacman(j, i);
                    std::cout << j << " " << i;
                    break;
            }
        }
    }
    updateFrame();
}

void LoadTextures(){
    textures.pellets.loadFromFile("textures/dots.png");
    textures.sprites.loadFromFile("textures/sprites.png");
    textures.map.loadFromFile("textures/map.png");
    textures.mapWhite.loadFromFile("textures/map_white.png");
    textures.font.loadFromFile("textures/font.png");
}

void Start() {
    LoadTextures();
    SetMap();
    SetObjects();
}

void MainLoop() {
    if (gameState.state == play) {
        gameState.pacman->update();
        int currentLives = gameState.lives;
        for (int i = 0; i < 4; ++i) {
            gameState.ghosts[i]->update();
            if (currentLives != gameState.lives) {
                if (gameState.lives <= 0) {
                    gameState.pauseTime = 20000;
                    gameState.state = over;
                } else {
                    gameState.pauseTime = 20000;
                    gameState.state = lose;
                }
                break;
            }
        }
    }
    updateFrame();
}

void StartLoop() {
    gameState.pauseTime -= FRAME_TIME;
    if (gameState.pauseTime <= 0) {
        gameState.state = play;
    }
    std::cout << "START\n";
    drawStart();
    updateFrame();
}

void LoseLoop() {//everything stops and pacman is dying
    gameState.pauseTime -= FRAME_TIME;
    if (gameState.pauseTime <= 0) {
        gameState.pauseTime = 20000;
        if(gameState.lives <= 0) {
            gameState.state = over;
        } else {
            SetObjects();
            gameState.state = start;
        }
    }
    updateFrame();
}

void OverLoop() {// game over
    gameState.pauseTime -= FRAME_TIME;
    if (gameState.pauseTime <= 0) {
        gameState.state = close;
    }
    drawOver();
    updateFrame();
}

void drawMapBlink(){

}

void updateFrameWin(){
    drawPacman();
    drawMapBlink();
}

void WinLoop() {//remove all ghosts and flash map
    gameState.pauseTime -= FRAME_TIME;
    if (gameState.pauseTime <= 0) {
        Start();
        gameState.pauseTime = 20000;
        gameState.state = start;
    }
    updateFrameWin();
}

void GameLoop() {
    switch (gameState.state) {
        case play:
            MainLoop();
            break;
        case start:
            StartLoop();
            break;
        case lose:
            LoseLoop();
            break;
        case over:
            OverLoop();
            break;
        case win:
            WinLoop();
            break;
        case close:
            updateFrame();
            break;
    }
}