

#ifndef NOTA_H
# define NOTA_H


struct Nota {
    int time;      // tempo in millisecondi in cui la nota deve apparire
    char key;        // il tasto associato (es. 'a', 's', 'd', 'f')
    int lasts;      //nota da tener premuto
    bool hit;        // se è stata colpita o no
    bool hold;

    Nota(int t, int k, char l) : time(t), key(k), lasts(l), hit(false), hold(false) {}
};


#endif
