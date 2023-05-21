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
    int getId(){return id;};
    float getPeso(){return peso;};
    int getGrauEntrada(){return grauEntrada;};
    int getGrauSaida(){return grauSaida;};

    No* getProx(){return proximo;};
    void setProx(No *no){
        this->proximo = no;
    }

    Arco* getArcoAdjacente(){return adjacente;};
    void setArcoAdjacente(Arco *arco){
        this->adjacente = arco;
    }
};

#endif // NO_H