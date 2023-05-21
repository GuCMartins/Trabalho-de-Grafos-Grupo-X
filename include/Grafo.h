#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include "Arco.h"

class Grafo
{
private:
    int ordem;
    //int regularidade;
    No* noInicial;
    bool direcionado, pondAresta, pondNode;
    void auxInserirArco(int idNoOrigem, int idNoDestino, float pesoArco);
public:
    Grafo(int ordem, bool direc, bool pondAresta, bool pondNode);
    ~Grafo();
    void inserirNo(int idNode,  float pesoNode);
    void removerNo(int idNode);
    
    void inserirArco(int idNoOrigem, int idNoDestino, float pesoArco);
    void removerArco(int idNoOrigem, int idNoDestino);
    int getOrdem(){return ordem;};
    No* getNoInicial(){return noInicial;};
    void imprimirListaNos();
    void imprimirListaNosAdjacentes(int idNo);

    No* findNoById(int id);
};
#endif // GRAFO_H