#ifndef ARCO_H
#define ARCO_H

#include "No.h"

class Arco
{
private:
    float pesoArco;
    Arco *proxArco;
    No *noOrigem;
    No *noDestino;

public:
    Arco(float peso, Arco *prox, No *origem, No *destino);
    ~Arco();
    Arco *getProximo();
    float getPeso();
    No* getNoOrigem(){return noOrigem;}
    No* getNoDestino(){return noDestino;}
};

#endif // Arco_H