#ifndef PACMAN_CELL_H
#define PACMAN_CELL_H

enum CellType{
    wall,
    pellet,
    energizer,
    door,
    empty
};

struct Cell{
    std::pair<int, int> pos;
    CellType type;

    Cell() = default;

    Cell(std::pair<int, int> newPos, CellType newType): pos(newPos), type(newType){}
};

#endif //PACMAN_CELL_H
