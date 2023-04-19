#ifndef ARCO_H
#define ARCO_H

#include "No.h"

class Arco
{
private:
    int pesoArco;
    Arco *proxArco;
    No *noOrigem;
    No *noDestino;

public:
    Arco(int peso, Arco *prox, No *origem, No *destino);
    ~Arco();
    int getPeso();
    int getNoOrigem();
    int getNoDestino();
};

#endif // Arco_H