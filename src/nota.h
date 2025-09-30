

#ifndef NOTA_H
# define NOTA_H


struct Nota {
    int time;      // tempo in millisecondi in cui la nota deve apparire
    char key;        // il tasto associato (es. 'a', 's', 'd', 'f')
    bool hit;        // se è stata colpita o no

    Nota(int t, char k) : time(t), key(k), hit(false) {}
};


#endif
