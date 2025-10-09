#ifndef GAME_H
# define GAME_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "nota.h"
#include <SFML/Graphics.hpp> 

#define WINDOW_X 800.f
#define WINDOW_Y 600.f
#define TARGET_Y 500.f

using namespace std;

class Game {
private:
    std::vector<Nota> notes;  // tutte le note della beatmap
    int hitCount = 0;         // numero di hit
    int missCount = 0;        // numero di miss

    // Funzioni interne per organizzare il codice
    void drawnotes(sf::RenderWindow& window, int elapsedMs, float noteSpeed);
    void updateNotes(int elapsedMs);
    void checkPress(int elapsedMs);
    void checkRelease(int elapsedMs);
    void handlePress(char key, int elapsedMs);
    void handleRelease(char key, int elapsedMs);

public:
    void loadBeatmap(const std::string& filename);
    void run();
};

#endif
