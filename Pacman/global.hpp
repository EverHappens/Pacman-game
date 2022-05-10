
constexpr unsigned char MOVEMENT_SPEED = 10;
constexpr unsigned char CELL_SIZE = 16 * 5;

constexpr unsigned int Y_OFFSET = 3 * CELL_SIZE;

constexpr unsigned char PELLET_SIZE = CELL_SIZE / 5;
constexpr unsigned int ENERGIZER_TIME = 30000;
constexpr unsigned int FRAME_TIME = 60;
constexpr unsigned char MAP_HEIGHT = 21;
constexpr unsigned char MAP_WIDTH = 21;
const std::array<std::string, MAP_WIDTH> map = {
        " ################### ",
        " #........#........# ",
        " #o##.###.#.###.##o# ",
        " #.................# ",
        " #.##.#.#####.#.##.# ",
        " #....#...#...#....# ",
        " ####.### # ###.#### ",
        "    #.#   0   #.#    ",
        "#####.# ##=## #.#####",
        "     .  #123#  .     ",
        "#####.# ##### #.#####",
        "    #.#       #.#    ",
        " ####.# ##### #.#### ",
        " #........#........# ",
        " #.##.###.#.###.##.# ",
        " #o.#.....P.....#.o# ",
        " ##.#.#.#####.#.#.## ",
        " #....#...#...#....# ",
        " #.######.#.######.# ",
        " #.................# ",
        " ################### "
};