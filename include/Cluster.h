#ifndef CLUSTER_H
#define CLUSTER_H

#include "Grafo.h"
#include <forward_list>
#include <string>

class Cluster
{
private:
    float min, max; // min <= Sum(Peso dos nós do cluster) <= max (Constraint do problema)
    Grafo *cluster_graph; //Grafo que será induzido pelos nós que já se encontram no cluster
    std::forward_list<int> inseridos; //Guarda o valor inteiro dos ids dos nós que já estão dentro do cluster
    float sumVertices, sumArestas; //Soma dos pesos dos nós do cluster e soma dos pesos das arestas do cluster
    bool dentroIntervalo; //Verifica a constraint do intervalo
    bool confereNosInseridos(int val); //Função auxiliar na inserção
    std::string type;
    int numVertices;
public:
    Cluster(float min, float max, Grafo *grafo, std::string type);
    ~Cluster();

    std::forward_list<int> getInseridos() {return inseridos;};

    int getMin(){return min;};
    int getMax(){return max;};
    std::string getType() {return this->type;};
    bool getDentroIntervalo(){
        if(this->sumVertices <= max && this->sumVertices >= min) 
            return true; 
        else 
            return false;
    };

    float getSumVertices(){return sumVertices;};
    float getSumArestas(){return sumArestas;};

    void zeraCluster(Grafo *grafo);

    int getOrdem(){return cluster_graph->getOrdem();};

    void inserirNoCluster(No* nodeToInsert); //Nó que será adicionado ao cluster

    Grafo *getGrafo(){return cluster_graph;};

    //Métodos do Gustavo.

    // bool setSumVertices(int idNodeFonte, int idNode,  float pesoNode);

    // void setSumArestas(float pesoAresta){this->SArestas += pesoAresta;};

    // int* melhorEscolha(Grafo* grafoGeral);//primeira posicao é o id do no fonte e a segunda é o id do no a ser adicionado

    //     //funcao que escolhe o no a ser adicionado em seguida baseado no peso da aresta
    
    void imprimirListaNos(){
        this->cluster_graph->imprimirListaNos();
    };
    void imprimirListaNosAdjacentes(int idNo){
        this->cluster_graph->imprimirListaNosAdjacentes(idNo);
    }
    void imprimirTodosNosAdjacentes(){
        this->cluster_graph->imprimirTodosNosAdjacentes();
    }
};
#endif // CLUSTER_H