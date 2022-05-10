#include <iostream>
#include <SFML/graphics.hpp>
#include <chrono>
#include "game.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(MAP_HEIGHT * CELL_SIZE, MAP_WIDTH * CELL_SIZE + Y_OFFSET * 2), "Pacman");
    window.setFramerateLimit(60);
    gameState.window = &window;
    Start();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if(gameState.state == close){
            window.close();
        }
        window.clear();
        GameLoop();
        window.display();
    }
}
