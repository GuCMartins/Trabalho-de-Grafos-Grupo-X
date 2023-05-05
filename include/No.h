#ifndef NO_H
#define NO_H

#include "Arco.h"

class No
{
private:
    int id;
    int grauEntrada;
    int grauSaida;
    // int x,y; //possivelmente usado para pontos no espaço R2
    float peso;
    No* proximo;
    Arco* adjacente;
public:
    No(int id, int x, int y, float peso);
    ~No();
    int getGrauEntrada(){return grauEntrada;};
    int getGrauSaida(){return grauSaida;};
    No* getProx(){no = proximo;};
};

#endif // NO_H