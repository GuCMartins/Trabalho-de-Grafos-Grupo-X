#ifndef NO_H
#define NO_H

#include "Arco.h"

class No
{
private:
    int id;
    bool visitado;
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
};

#endif // NO_H