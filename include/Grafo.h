#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include "Arco.h"

class Grafo
{
private:
    int ordem, numArcos;
    //int regularidade;
    No* noInicial;
    bool direcionado, pondAresta, pondNode;
    void auxInserirArco(No* noDestino, No* noOrigem, float pesoArco);
    void auxRemoverArco(No* noDestino, int idNoOrigem);
    int** matrizAdj;
public:
    Grafo(int ordem, bool direc, bool pondAresta, bool pondNode);
    ~Grafo();
    void inserirNo(int idNode,  float pesoNode);
    void removerNo(int idNode);

    void inserirArco(int idNoOrigem, int idNoDestino, float pesoArco);
    void removerArco(int idNoOrigem, int idNoDestino);
    void setOrdem(int ordem){this->ordem = ordem;};
    int getOrdem(){return ordem;};
    No* getNoInicial(){return noInicial;};
    void imprimirListaNos();
    void imprimirListaNosAdjacentes(int idNo);
    void imprimirTodosNosAdjacentes();

    bool ehDirecionado() {return direcionado;};
    void setNumArcos(int numArcos) { this->numArcos = numArcos;};
    int getNumArcos() {return numArcos;};

    No* findNoById(int id);
    bool ehDir(){return direcionado;};
    bool ehPondAr(){return pondAresta;};
    bool ehPondNode(){return pondNode;};

    bool existeArco(int noPartida,int noDestino);
    int* vizinhancaAberta(int idNo);
    int* vizinhancaFechada(int idNo);
    bool Euleriano(int* visitados);
};
#endif // GRAFO_H