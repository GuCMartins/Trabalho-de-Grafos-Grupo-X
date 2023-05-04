#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include "Arco.h"

class Grafo
{
private:
    int ordem;
    //int regularidade;
    No* NoInicial;
    int numArcos;
    bool direcionado, pondAresta, pondNode;
public:
    Grafo(int n, bool direc, bool pondAresta, bool pondNode);
    ~Grafo();
    void inserirNo(int idNode,  float pesoNode);
    void removerNo(int idNode);
    void inserirArco(int idNoOrigem, int idNoDestino, float pesoArco);
    void removerArco(int idNoOrigem, int idNoDestino);
    int* getGrauNo(int idNode);
    int getOrdem(){return ordem;};
    int getNumArcos(){return numArcos;};
};
#endif // GRAFO_H