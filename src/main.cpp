#include "Game.h"

int main() {
    Game game;
    game.loadBeatmap("beatmaps/song1.txt");

    if (!game.loadFont("assets/font/VIDEOPHREAK.ttf")) {
    std::cerr << "Failed to load font!" << std::endl;
    }
    game.run();
    return 0;
}
