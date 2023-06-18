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
    for(int i = 0; i < num_clusters; i++){
        if(!clusters[i]->getDentroIntervalo())
            return false;
    }
    return true;
}

bool compareNo(No *a, No *b) {
    return a->grauEntrada() <= b.grauEntrada();
}

forward_list ordenaGrafoPorGrau(Grafo *g){
    std::forward_list<No*> nos;
    for(No *no=g->getNoInicial();no!NULL;no=no->getProx()){
        nos->push_front(no);
    }
    
    nos.sort(compareNo);
    return nos;
}

void guloso(Grafo *g, Cluster* clusters, int num_clusters){
    int cont = 0;
    std::forward_list<No*> listaGraus = ordenaGrafoPorGrau(g); 
    while(cont < g->getOrdem() && solucao(clusters, num_clusters)){
        if(cont < num_clusters){
            clusters[cont]->inserirNoCluster(g->findNoById(listaGraus.push_front();))
            listaGraus.pop_front();
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