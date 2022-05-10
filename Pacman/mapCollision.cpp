#include "mapCollision.h"

Cell collidesWith(float x, float y) {
    std::pair<int, int> cells[4];
    Cell cell;
    bool isDoor = false;
    for (int i = 0; i < 4; ++i) {
        int cellX, cellY;
        switch (i) {
            case 0:
                cellX = ceil(x / static_cast<float>(CELL_SIZE));
                cellY = ceil(y / static_cast<float>(CELL_SIZE));
                break;
            case 1:
                cellX = ceil(x / static_cast<float>(CELL_SIZE));
                cellY = floor(y / static_cast<float>(CELL_SIZE));
                break;
            case 2:
                cellX = floor(x / static_cast<float>(CELL_SIZE));
                cellY = ceil(y / static_cast<float>(CELL_SIZE));
                break;
            case 3:
                cellX = floor(x / static_cast<float>(CELL_SIZE));
                cellY = floor(y / static_cast<float>(CELL_SIZE));
                break;
        }
        if (gameState.currentMap[cellY][cellX] == '#') {
            cell.pos = {cellY, cellX};
            cell.type = CellType::wall;
            break;
        }
        if (gameState.currentMap[cellY][cellX] == '.') {
            cell.pos = {cellY, cellX};
            cell.type = CellType::pellet;
        }
        if(gameState.currentMap[cellY][cellX] == 'o'){
            cell.pos = {cellY, cellX};
            cell.type = CellType::energizer;
        }
        if (gameState.currentMap[cellY][cellX] == '=') {
            isDoor = true;
            cell.pos = {cellY, cellX};
            cell.type = CellType::door;
        }
    }
    if(isDoor && cell.type != wall){
        cell.type = door;
    }
    return cell;
}