

#ifndef NOTA_H
# define NOTA_H

enum HitResult {
    NOT_HIT,    //1
    HOLDING_HIT,    //2
    EARLY_HIT,  //3
    OK_HIT, //4
    PERFECT_HIT,    //5  
    LATE_HIT    //6
};

struct Nota {
    int time;      // tempo in millisecondi in cui la nota deve apparire
    char key;        // il tasto associato (es. 'a', 's', 'd', 'f')
    int lasts;      //nota da tener premuto
    HitResult hit;        // se è stata colpita e quanto bene
    bool hold;

    Nota(int t, char k, int l) : time(t), key(k), lasts(l), hit(NOT_HIT), hold(false) {}

    bool missed() const {
        return hit == EARLY_HIT || hit == LATE_HIT;
    }   

};


#endif
