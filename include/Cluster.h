#ifndef CLUSTER_H
#define CLUSTER_H

#include "Grafo.h"
#include "No.h"
#include "Arco.h"

class Cluster
{

private:
    int min, max;
    bool dentroIntervalo = false;
    Grafo *grafo;
    float SVertices = 0, SArestas = 0;
public:
    Cluster(int min, int max, Grafo *grafo);
    ~Cluster();

    int getMin(){return min;};
    int getMax(){return max;};
    float getSVertices(){return SVertices;};
    float getSArestas(){return SArestas;};
    bool getDentroIntervalo(){return dentroIntervalo;};
    Grafo *getGrafo(){return grafo;};

    bool setSVertices(int* NodeOriDest, Grafo *grafoGeral);

    void setSArestas(float pesoAresta){this->SArestas += pesoAresta;};

    int getOrdem(){return grafo->getOrdem();};

    int* melhorEscolha(Grafo* grafoGeral);//primeira posicao é o id do no fonte e a segunda é o id do no a ser adicionado
        //funcao que escolhe o no a ser adicionado em seguida baseado no peso da aresta

    void imprimirListaNos();
    void imprimirListaNosAdjacentes(int idNo);
    void imprimirTodosNosAdjacentes();
};
#endif // CLUSTER_H