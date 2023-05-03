#ifndef NO_H
#define NO_H

#include "Arco.h"

class No
{
private:
    int id;
    int grauEntrada;
    int grauSaida;
    int x,y;
    float peso;
    No *proximo;
    Arco *adjacente;
public:
    No(int id, int x, int y, float peso);
    ~No();
    bool confirmDigrafo();
    int getId();
    int getX();
    int getY();
    float getPeso();
};

#endif // NO_H