#include "pacman.h"
#include <iostream>

float MOVEMENT_SHIFT = 10.f;

Ghost::Ghost() = default;

Ghost::Ghost(unsigned int x, unsigned int y, GhostType newGhostType, bool canUseHome) : Ghost() {
    pos = {static_cast<float>(x * CELL_SIZE), static_cast<float>(y * CELL_SIZE)};
    useHome = canUseHome;
    ghostType = newGhostType;
}

GhostType Ghost::getType() {
    return ghostType;
}

sf::Vector2f Ghost::getPos() {
    return pos;
}

Pacman::Pacman() = default;

Pacman::Pacman(unsigned int x, unsigned int y) : Pacman() {
    pos = {static_cast<float>(x * CELL_SIZE), static_cast<float>(y * CELL_SIZE)};
}

sf::Vector2f Pacman::getPos() {
    return pos;
}

Direction Pacman::getDir() {
    return dir;
}

bool Pacman::isStuck() {
    sf::Vector2f cells[2];
    switch (dir) {
        case up:
            cells[0] = {floor(pos.x / static_cast<float>(CELL_SIZE)), floor(pos.y / static_cast<float>(CELL_SIZE))};
            cells[1] = {ceil(pos.x / static_cast<float>(CELL_SIZE)), floor(pos.y / static_cast<float>(CELL_SIZE))};
            if (map[cells[0].y][cells[0].x] == '#' || map[cells[1].y][cells[1].x] == '#')
                return true;
            break;
        case down:
            cells[0] = {floor(pos.x / static_cast<float>(CELL_SIZE)), ceil(pos.y / static_cast<float>(CELL_SIZE))};
            cells[1] = {ceil(pos.x / static_cast<float>(CELL_SIZE)), ceil(pos.y / static_cast<float>(CELL_SIZE))};
            if (map[cells[0].y][cells[0].x] == '#' || map[cells[1].y][cells[1].x] == '#')
                return true;
            break;
        case left:
            cells[0] = {floor(pos.x / static_cast<float>(CELL_SIZE)), floor(pos.y / static_cast<float>(CELL_SIZE))};
            cells[1] = {floor(pos.x / static_cast<float>(CELL_SIZE)), ceil(pos.y / static_cast<float>(CELL_SIZE))};
            if (map[cells[0].y][cells[0].x] == '#' || map[cells[1].y][cells[1].x] == '#')
                return true;
            break;
        case right:
            cells[0] = {ceil(pos.x / static_cast<float>(CELL_SIZE)), floor(pos.y / static_cast<float>(CELL_SIZE))};
            cells[1] = {ceil(pos.x / static_cast<float>(CELL_SIZE)), ceil(pos.y / static_cast<float>(CELL_SIZE))};
            if (map[cells[0].y][cells[0].x] == '#' || map[cells[1].y][cells[1].x] == '#')
                return true;
            break;
        case none:
            break;
    }
    return false;
}

void Pacman::update() {
    Direction new_dir = none;
    Cell collisionWith;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        new_dir = up;
        collisionWith = collidesWith(pos.x, pos.y - MOVEMENT_SHIFT);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        new_dir = down;
        collisionWith = collidesWith(pos.x, pos.y + MOVEMENT_SHIFT);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        new_dir = left;
        collisionWith = collidesWith(pos.x - MOVEMENT_SHIFT, pos.y);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        new_dir = right;
        collisionWith = collidesWith(pos.x + MOVEMENT_SHIFT, pos.y);
    }
    dir = new_dir;
    if (collisionWith.type != CellType::wall && collisionWith.type != CellType::door) {
        switch (dir) {
            case up:
                pos.y -= MOVEMENT_SHIFT;
                break;
            case down:
                pos.y += MOVEMENT_SHIFT;
                break;
            case left:
                pos.x -= MOVEMENT_SHIFT;
                break;
            case right:
                pos.x += MOVEMENT_SHIFT;
                break;
            case none:
                break;
        }
    }
    if (gameState.energizerTime > 0) {
        gameState.energizerTime -= FRAME_TIME;
        if (gameState.energizerTime <= 0) {
            for (int i = 0; i < 4; ++i) {
                gameState.ghosts[i]->unscatter();
            }
        }
    }
    if (collisionWith.type == CellType::pellet) {
        gameState.currentMap[collisionWith.pos.first][collisionWith.pos.second] = '0';
    }
    if (collisionWith.type == CellType::energizer) {
        gameState.currentMap[collisionWith.pos.first][collisionWith.pos.second] = '0';
        gameState.energizerTime = ENERGIZER_TIME;
        for (int i = 0; i < 4; ++i) {
            gameState.ghosts[i]->makeScatter();
        }
    }
    if (-CELL_SIZE >= pos.x)
        pos.x = CELL_SIZE * MAP_WIDTH - MOVEMENT_SHIFT;
    else if (CELL_SIZE * MAP_WIDTH <= pos.x)
        pos.x = MOVEMENT_SHIFT - CELL_SIZE;
}

sf::Vector2f getTargetCell(sf::Vector2f coord) {

}

void Ghost::makeScatter() {
    if (behaviorType != frightened) {
        behaviorType = scatter;
        movementSpeed /= 2;
    }
}

void Ghost::unscatter() {
    if (behaviorType != frightened) {
        behaviorType = chase;
        movementSpeed = MOVEMENT_SPEED;
    }
}

sf::Vector2f Ghost::getChaseTarget() {
    sf::Vector2f pacmanPos = gameState.pacman->getPos();
    switch (ghostType) {
        case blinky:
            return pacmanPos;
            break;
        case pinky:
            switch (gameState.pacman->getDir()) {
                case up:
                    return {pacmanPos.x, pacmanPos.y - 2 * CELL_SIZE};
                    break;
                case down:
                    return {pacmanPos.x, pacmanPos.y + 2 * CELL_SIZE};
                    break;
                case left:
                    return {pacmanPos.x - 2 * CELL_SIZE, pacmanPos.y};
                    break;
                case right:
                    return {pacmanPos.x + 2 * CELL_SIZE, pacmanPos.y};
                    break;
                case none:
                    return pacmanPos;
                    break;
            }
            break;
        case inky:
            return pacmanPos + pacmanPos - gameState.ghosts[0]->getPos();//the ghost must be blinky
            break;
        case clyde:
            if (sqrt(pow(pacmanPos.x - pos.x, 2) + pow(pacmanPos.y - pos.y, 2)) > 8 * CELL_SIZE) {
                return pacmanPos;
            } else behaviorType = scatter;
            break;
    }
    return {-1, -1};
}

sf::Vector2f Ghost::getScatterTarget() {
    sf::Vector2f targetPos;
    switch (ghostType) {//change if the map is different
        case blinky:
            targetPos = {(MAP_WIDTH - 3) * CELL_SIZE, 1 * CELL_SIZE};
            break;
        case pinky:
            targetPos = {2 * CELL_SIZE, 1 * CELL_SIZE};
            break;
        case inky:
            targetPos = {(MAP_WIDTH - 3) * CELL_SIZE, (MAP_HEIGHT - 2) * CELL_SIZE};
            break;
        case clyde:
            if (gameState.energizerTime <= 0 &&
                sqrt(pow(gameState.pacman->getPos().x - pos.x, 2) + pow(gameState.pacman->getPos().y - pos.y, 2)) >=
                8 * CELL_SIZE) {
                behaviorType = chase;
                targetPos = {-1, -1};
            } else targetPos = {2 * CELL_SIZE, (MAP_HEIGHT - 2) * CELL_SIZE};
            break;
    }
    return targetPos;
}

sf::Vector2f Ghost::getFrightenedTarget() {

}

Direction Ghost::bestDir(sf::Vector2f target) {
    std::pair<float, Direction> minDist = {10000000, none};
    Cell bestCell = Cell({-1, -1}, empty);
    Cell collisionWith;
    float currDistance;
    for (int i = 0; i < 4; ++i) {
        switch (i) {
            case 0:
                if (dir == down) continue;
                collisionWith = collidesWith(pos.x, pos.y - movementSpeed);
                if (collisionWith.type != CellType::wall && (collisionWith.type != CellType::door || useHome)) {
                    std::cout << "go up?\n";
                    if (useHome && behaviorType != frightened) {
                        Cell currentCollision = collidesWith(pos.x, pos.y);
                        if (currentCollision.type == door && collisionWith.type != door) useHome = false;
                    }
                    currDistance = sqrt(pow(target.x - pos.x, 2) + pow(target.y - pos.y + movementSpeed, 2));
                    if (currDistance <= minDist.first) {
                        bestCell = collisionWith;
                        minDist = {currDistance, up};
                    }
                }
                break;
            case 1:
                if (dir == up) continue;
                collisionWith = collidesWith(pos.x, pos.y + movementSpeed);
                if (collisionWith.type != CellType::wall && (collisionWith.type != CellType::door || useHome)) {
                    currDistance = sqrt(pow(target.x - pos.x, 2) + pow(target.y - pos.y - movementSpeed, 2));
                    if (currDistance <= minDist.first) {
                        bestCell = collisionWith;
                        minDist = {currDistance, down};
                    }
                }
                break;
            case 2:
                if (dir == right) continue;
                collisionWith = collidesWith(pos.x - movementSpeed, pos.y);
                if (collisionWith.type != CellType::wall && (collisionWith.type != CellType::door || useHome)) {
                    currDistance = sqrt(pow(target.x - pos.x + movementSpeed, 2) + pow(target.y - pos.y, 2));
                    if (currDistance <= minDist.first) {
                        minDist = {currDistance, left};
                    }
                }
                break;
            case 3:
                if (dir == left) continue;
                collisionWith = collidesWith(pos.x + movementSpeed, pos.y);
                if (collisionWith.type != CellType::wall && (collisionWith.type != CellType::door || useHome)) {
                    currDistance = sqrt(pow(target.x - pos.x - movementSpeed, 2) + pow(target.y - pos.y, 2));
                    if (currDistance <= minDist.first) {
                        minDist = {currDistance, right};
                    }
                }
                break;
        }
    }
    return minDist.second;
}

void Ghost::update() {
    std::pair<int, int> coord;
    sf::Vector2f targetPos;
    switch (behaviorType) {
        case chase:
            targetPos = getChaseTarget();//chase pacman
            dir = bestDir(targetPos);//kostil??? behaviorType change!!!
            break;
        case scatter:
            targetPos = getScatterTarget();//run to corners when pman is energized
            dir = bestDir(targetPos);
            break;
        case frightened://when dead
            targetPos = {11 * CELL_SIZE, 9 * CELL_SIZE};
            if (gameState.energizerTime > 0) {
                if (sqrt(pow(pos.x - targetPos.x, 2) + pow(pos.y - targetPos.y, 2)) <= movementSpeed) {
                    pos = targetPos;//we need to fix its position
                    movementSpeed *= 2;
                    behaviorType = chase;
                    useHome = true;
                }
            } else {
                behaviorType = chase;
                movementSpeed = MOVEMENT_SHIFT;
            }
            dir = bestDir(targetPos);
            break;
    }
    switch (dir) {//we already checked for collision, so it's okay
        case up:
            pos.y -= movementSpeed;
            break;
        case down:
            pos.y += movementSpeed;
            break;
        case left:
            pos.x -= movementSpeed;
            break;
        case right:
            pos.x += movementSpeed;
            break;
        case none:
            break;
    }
    if (-CELL_SIZE >= pos.x) pos.x = CELL_SIZE * MAP_WIDTH - movementSpeed;
    else if (CELL_SIZE * MAP_WIDTH <= pos.x) pos.x = movementSpeed - CELL_SIZE;
    if (sqrt(pow(pos.x - gameState.pacman->getPos().x, 2) + pow(pos.y - gameState.pacman->getPos().y, 2)) <
        CELL_SIZE) {//if pacman and ghost collide
        if (gameState.energizerTime > 0) {
            behaviorType = frightened;
            useHome = true;
        } else --gameState.lives;
    }
}
