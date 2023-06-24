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
        if(!((clusters[i]).getDentroIntervalo()))
            return false;
    }
    return true;
}

typedef struct{
    int NoOrigem, NoDest;
    float pesoAresta;
} NoNoArco;

bool compareArco(NoNoArco *a, NoNoArco *b) {
    return a->pesoAresta <= b->pesoAresta;
}

std::forward_list<NoNoArco> ordenaGrafoPorPesoAresta(Grafo *g,int max,int **matriz){
    std::forward_list<NoNoArco> OriDestPeso;
    No *noOrigem;
    for(int i = 0; i < g->getOrdem(); i++){
        noOrigem = g->findNoById(i);
        Arco *arco = noOrigem->getAdjacentes();
        while(arco != nullptr){//analisar apenas a parte superior triangular superior da matriz para nao inserir arestas repetidas
            if(arco->getNodeDest() > noOrigem->getId()){
                if(g->findNoById(i)->getPeso() + g->findNoById(arco->getNodeDest())->getPeso() <= max){//caso os dois nos que compoem a aresta estejam dentro do intervalo
                    NoNoArco objeto;
                    objeto.NoOrigem = noOrigem->getId();
                    objeto.NoDest = arco->getNodeDest();
                    objeto.pesoAresta = arco->getPeso();
                    OriDestPeso.push_front(objeto);
                    matriz[i][arco->getNodeDest()] = arco->getPeso();
                }
                break;
            }
            arco = arco->getProx();
        }
    }

    OriDestPeso.sort(compareArco);
    return OriDestPeso;
}

void guloso(Grafo *g, Cluster* clusters, int num_clusters,int min,int max){
    int cont = 0;
    int **matriz = new int*[g->getOrdem()];
    for(int i = 0; i < g->getOrdem(); i++){
        matriz[i] = new int[g->getOrdem()];
        for(int j = 0; j < g->getOrdem(); j++){
            matriz[i][j] = 0;
        }
    }
    int *nosInseridos = new int[g->getOrdem()];
    for(int i = 0; i < g->getOrdem(); i++){
        nosInseridos[i] = -1;
    }  
    std::forward_list<NoNoArco> listaArestaPorPeso = ordenaGrafoPorPesoAresta(g,max,matriz); //obtem as arestas viaveis ordenadas por peso
    //a partir daqui, tem-se:
    //listaArestaPorPeso: lista de arestas ordenadas por peso
    //matriz: matriz de adjacencia do grafo g, onde cada posicao [i][j] contem o peso da aresta que liga o no i ao no j,sendo que peso(i) + peso(j) <= max
    while(cont < g->getOrdem() && !(solucao(clusters, num_clusters))){
        if(cont < num_clusters){
            while(nosInseridos[listaArestaPorPeso.front().NoOrigem] || nosInseridos[listaArestaPorPeso.front().NoDest]){
                listaArestaPorPeso.pop_front();
            }
            clusters[cont].inserirNoCluster(g->findNoById(listaArestaPorPeso.front().NoOrigem));
            nosInseridos[listaArestaPorPeso.front().NoOrigem] = cont;
            clusters[cont].inserirNoCluster(g->findNoById(listaArestaPorPeso.front().NoDest));
            nosInseridos[listaArestaPorPeso.front().NoDest] = cont;
            listaArestaPorPeso.pop_front();
        }else{
            int i = 0;
            float *PontosCluster = new float[num_clusters];
            while(nosInseridos[i] == -1){
                i++;
            }
            for(int j = 0; j <g->getOrdem(); j++){
                if(nosInseridos[j] != -1 && clusters[nosInseridos[j]].getSumVertices() + g->findNoById(i)->getPeso() <= max){
                    PontosCluster[nosInseridos[j]] += matriz[i][j];
                }
            }
            int ClustercomMais = 0;
            for(int j = 0; j < num_clusters; j++){
                if(PontosCluster[j] > PontosCluster[ClustercomMais]){
                    ClustercomMais = j;
                }
            }
            clusters[ClustercomMais].inserirNoCluster(g->findNoById(i));
        }
        cont++;
    }
}



//Algoritmo Guloso Randomizado
void gulosoRandomizado(){
// int nosNaoInseridos = 0;
// for(int i = 0; i < g->getOrdem(); i++){
//     if(nosInseridos[i] == -1){
//         nosNaoInseridos++;
//     }
// }
// float **matrizDecisao = new float*[nosNaoInseridos];
// for(int i = 0; i < nosNaoInseridos; i++){
//     matrizDecisao[i] = new float[num_clusters];
//     for(int j = 0; j < num_clusters; j++){
//         matrizDecisao[i][j] = 0;
//     }
// }
}



//Algoritmo Guloso Randomizado Reativo
void gulosoRandomizadoReativo(){

}



#endif