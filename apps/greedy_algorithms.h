#ifndef GREEDY_ALGORITHMS_CPP
#define GREEDY_ALGORITHMS_CPP

#include "../include/Grafo.h"
#include "../include/Cluster.h"
#include <forward_list>

//Algoritmo Guloso

/*

Pseudocódigo para nos auxiliar a fazer o código!

C: Conjunto de candidatos
S: Conjunto que irá conter a solução

função AlgoritmoGuloso(C: conjunto)
    S <- Vazio

    enquanto C != Vazio e não solução(S) faça
        x <- seleciona C
        C <- C{x}
        se é viável S União {x} então
            S <- S União {x}
        fim se
    fim enquanto

    se solução(S) então
        retorne S
    senão
        retorne "Não existe solução!"
    fim se
fim função

*/

/*
    Dado um grafo g e um vetor com os clusters, essa função será capaz de devolver em um arquivo .txt uma solução aproximada para o problema de clusterização capacitada. Nesse arquivo, estarão contidos a soma dos pesos das arestas de cada cluster, e os nós que cada cluster possui. 

    :param g: Grafo que será analisado
    :param clusters: Vetor de clusters instanciado durante a leitura do arquivo de instâncias do problema
*/

bool solucao(Cluster* clusters, int num_clusters){
    clusters[0].getDentroIntervalo();
    for(int i = 0; i < num_clusters; i++){
        if(!(clusters[i].getDentroIntervalo()))
            return false;
    }
    return true;
}

bool compareArco(Arco *a, Arco *b) {
    return a->getPeso() <= b->getPeso();
}

std::forward_list<Arco*> ordenaGrafoPorPesoAresta(Grafo *g,int max){
    std::forward_list<Arco*> arcos;
    No* noOrigem, *noDestino;
    int** matriz = new int*[g->getOrdem()];
    for(int i = 0; i < g->getOrdem(); i++){
        matriz[i] = new int[g->getOrdem()];
    }
    for(int i = 0; i < g->getOrdem(); i++){
        noOrigem = g->findNoById(i);
        for(int j = 0; j < g->getOrdem(); j++){
            noDestino = g->findNoById(j);
            matriz[i][j] = 0;
            Arco *arco = noOrigem->getAdjacentes();
            while(arco != nullptr && j>i){//analisar apenas a parte superior triangular superior da matriz para nao inserir arestas repetidas
                if(arco->getNodeDest() == noDestino->getId()){
                    matriz[i][j] = arco->getPeso();
                    if(g->findNoById(i)->getPeso() + g->findNoById(j)->getPeso() <= max){//caso os dois nos que compoem a aresta estejam dentro do intervalo
                        arcos.push_front(arco);
                    }
                    break;
                }
                arco = arco->getProx();
            }
        }
    }

    arcos.sort(compareArco);
    return arcos;
}

void guloso(Grafo *g, Cluster* clusters, int num_clusters,int min,int max){
    int cont = 0;
    std::forward_list<Arco*> listaArestaPorPeso = ordenaGrafoPorPesoAresta(g,max); //obtem as arestas viaveis ordenadas por peso
    while(cont < g->getOrdem() && solucao(clusters, num_clusters)){
        if(cont < num_clusters){
            clusters[cont].inserirNoCluster(g->findNoById(listaArestaPorPeso.front()->getNodeDest()),listaArestaPorPeso.front());
            listaArestaPorPeso.pop_front();
        }else{
            //
        }
        cont++;
    }
}



//Algoritmo Guloso Randomizado
void gulosoRandomizado(){

}



//Algoritmo Guloso Randomizado Reativo
void gulosoRandomizadoReativo(){

}



#endif