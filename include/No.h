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
    Arco* adjacentes;//ponteiro para o primeiro arco adjacente ao no

public:
    No(int id, int x, int y, float peso);
    No(int id, float peso);
    ~No();
    int getX(){return x;};
    int getY(){return y;};
    No getProximo(){return *proximo;};
    Arco* getAdjacente(){return adjacentes;};
    int getId() { return id; };
    float getPeso() { return peso; };
    void incrementaGrauEntrada(int i) { this->grauEntrada += i; };
    void incrementaGrauSaida(int i) { this->grauSaida += i; };
    void decrementaGrauEntrada(int i) { this->grauEntrada -= i; };
    void decrementaGrauSaida(int i) { this->grauSaida -= i; };
    int getGrauEntrada() { return grauEntrada; };
    int getGrauSaida() { return grauSaida; };

    No *getProx() { return proximo; };
    void setProx(No *no){this->proximo = no;}

    Arco *getAdjacentes() { return adjacentes; };
    void setAdjacente(Arco *arco){ this->adjacentes = arco;}
};

#endif // NO_H