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
    Arco *getProximo(){return proxArco;}
    void setProximo(Arco *arco){proxArco = arco;}
    float getPeso(){return pesoArco;}
    void setPeso(float peso){pesoArco = peso;}
    No* getNoOrigem(){return noOrigem;}
    void setNoOrigem(No* no){noOrigem = no;}
    No* getNoDestino(){return noDestino;}
    void setNoDestino(No* no){noDestino = no;}
};

#endif // Arco_H