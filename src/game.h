#pragma once
#include <vector>
#include <string>
#include "note.h"
using namespace std;

class Game {
private:
    vector<Note> notes;   // tutte le note della beatmap
    int hitCount = 0;          // note colpite
    int missCount = 0;         // note mancate

public:
    void loadBeatmap(const std::string& filename);
    void run();
};
