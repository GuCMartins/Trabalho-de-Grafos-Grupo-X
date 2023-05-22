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
    Arco *getProx(){return proxArco;}
    void setProx(Arco *arco){proxArco = arco;}
    float getPeso(){return w;}
    void setPeso(float peso){w = peso;}
    No* getNoOrigem(){return noOrigem;}
    void setNoOrigem(No* no){noOrigem = no;}
    No* getNoDestino(){return noDestino;}
    int getNodeDest(){return nodeDest;}
    void setNoDestino(No* no){noDestino = no;}
};

#endif // Arco_H