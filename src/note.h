#pragma once

struct Note {
    int time;      // tempo in millisecondi in cui la nota deve apparire
    char key;        // il tasto associato (es. 'a', 's', 'd', 'f')
    bool hit;        // se è stata colpita o no

    Note(int t, char k) : time(t), key(k), hit(false) {}
};
