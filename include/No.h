#ifndef NO_H
#define NO_H

#include "Arco.h"

class No
{
private:
    int id;
    int grauEntrada;//se o grafo for direcionado, grau de entrada e saida podem ser diferentes. Se for direcionado, usar apenas o grau de entrada
    int grauSaida=0;
    int x,y; //possivelmente usado para pontos no espaço R2
    float peso;
    No* proximo;//ponteiro para o proximo no
    Arco* adjacente;//ponteiro para o primeiro arco adjacente ao no
public:
    No(int id, int x, int y, float peso);
    ~No();
    int getId();
    int getX();
    int getY();
    float getPeso();
    int getGrauEntrada(){return grauEntrada;};
    int getGrauSaida(){return grauSaida;};
    No getProximo(){return *proximo;};
    Arco* getAdjacente(){return adjacente;};
};

#endif // NO_H