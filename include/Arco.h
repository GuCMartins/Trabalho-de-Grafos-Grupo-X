#ifndef ARCO_H
#define ARCO_H

#include "No.h"

class Arco
{
private:
    No *noOrigem;
    No *noDestino;
//confirmar a parte abaixo
    int nodeDest;
    float w;
    Arco* proxArco;
public:
    Arco(int nodeDest, float w);
    ~Arco();
    float getPeso();

    int getNodeDest(){return nodeDest;};

    Arco * getProx(){return proxArco;}
    void setProx(Arco * arco){ this->proxArco = arco;}
};

#endif // Arco_H