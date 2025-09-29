#include "Game.h"

int main() {
    Game game;
    game.loadBeatmap("beatmaps/song1.txt");
    game.run();
    return 0;
}
