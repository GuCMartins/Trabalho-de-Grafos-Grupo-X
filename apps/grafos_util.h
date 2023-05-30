#ifndef GRAFOSUTIL_CPP
#define GRAFOSUTIL_CPP
#include <limits>
#include <algorithm>
#include <iostream>
#include "../include/Grafo.h"

using namespace std;    


/*
    => Verifico se a quantidade de arestas no grafo corresponde a n*(n-1)/2,
    visto que se um grafo é completo. Além disso, é necessário verificar se
    o grafo é multigrafo ou não, pois a primeira verificação fura se desconsi-
    derar multigrafos.
    => Exemplo: grafo k3, com arestas A, B e C. Se pegarmos um multigrafo A,B,C
    o mesmo não é completo mas satisfaz a primeira propriedade. Grafos com self-
    loops também caem no mesmo problema. Botar condição para isso depois.
*/
bool ehCompleto(Grafo *g){
    if(g->getNumArcos() == g->getOrdem()*(g->getOrdem()-1)/2)
        return true;
    else
        return false;
}

/*
    => Grau do grafo é o maior valor para de grau dentre os vértices do grafo.
    => Seria verificar em cada nó a quantidade de itens na lista de arcos dele. Caso
    alguém faça a parte de sequência de grau, seria interessante mudar essa função.
*/
int grauGrafo(Grafo* g){
    //testar se o grafo é direcionado;
    if(!g->ehDirecionado()){
        int max = -1;
        for (No *v = g->getNoInicial(); v != NULL; v = v->getProx())
        {
            if (v->getGrauEntrada() > max || max == -1)
                max = v->getGrauEntrada();
        }
        return max;
    }else{
        cout << "Grafo eh direcionado." << endl;
        return -1;
    }
}

bool ehNulo(Grafo *g){
    for (No* node = g->getNoInicial(); node != NULL; node = node->getProx()) 
        if(node->getGrauEntrada() != 0)
            return false;
    return true;
}

bool ehTrivial(Grafo *g){
    if(g->getOrdem() == 1)
        return true;
    else
        return false;
}

/* Parte do Algoritmo de Dijkstra */

int findDistMin(int* caminhoMinimo, int* visited, int tam){
    int min = -1;
    for(int i = 0; i < tam; i++){
        if((caminhoMinimo[i] < min || min == -1) && visited[i] == 0){
            min = i;
        }
    }
    return min;
}

int mapIndex(No* noInicial, int index){
    int mappedIndex = 0;
    while(noInicial != NULL){
        if(noInicial->getId() == index)
            return mappedIndex;
        mappedIndex++;
        noInicial = noInicial->getProx();
    }
    return -1;
}

int mapIndexReverse(No* noInicial, int index){
    for(int i = 0; i != index; i++){
        noInicial = noInicial->getProx();
    }
    return noInicial->getId();
}

int* dijkstraAlgorithm(Grafo *g){
    //O mapeamento envolve a ordem da lista encadeada de Nós.
    //O primeiro nó da lista encadeada, é o primeiro nó que estará como não visitado, o segundo nó da lista encad. é o segundo como não visitado, etc. 
    
    int* visited = new int[g->getOrdem()];
    int* caminhoMinimo = new int[g->getOrdem()];
    // int* noAnterior = new int[g->getOrdem()];

    No* noInicial = g->getNoInicial();

    for(int i = 0; i < g->getOrdem(); i++){
        if(i == 0){
            caminhoMinimo[i] = 0;
        }else{
            caminhoMinimo[i] = numeric_limits<int>::max();
        }
        visited[i] = 0; //0 significa que não foi visitado, e 1 o contrário.
        // noAnterior[i] = -1; //Iniciando com a flag -1, por que inicialmente eles não tem nó anterior
    }
    
    for(int i = 0; i < g->getOrdem(); i++){
        int j = findDistMin(caminhoMinimo, visited, g->getOrdem()); //Indice do cara com menor dist em relação ao nó inicial e que não foi visitado.
        
        visited[j] = 1; //Marco esse cara como visitado.
        
        cout << "Valor de j: " << j << endl;
        int indexList = mapIndexReverse(noInicial, j);

        cout << "Valor de indexList: " << indexList << endl;
        No* noAnalisado = g->findNoById(indexList);

        for(Arco *adjacentes = noAnalisado->getAdjacentes(); adjacentes != NULL; adjacentes = adjacentes->getProx()){
            int mappedIndex = mapIndex(noInicial, adjacentes->getNodeDest()); //Valor antigo
            float val1 = caminhoMinimo[mappedIndex];
            float val2 = adjacentes->getPeso() + caminhoMinimo[j];
            caminhoMinimo[mappedIndex] = min(val1, val2);
        } 
    }

    return caminhoMinimo;
}

/* Parte da Busca em Profundidade */

void DFSGeral(No *noPartida, Grafo *g, int* visitado){//o no de partida é iniciado como o no inicial do grafo
    visitado[noPartida->getId() - 1] = 1;
    Arco *arco = noPartida->getAdjacentes();

    while(arco != NULL){
        if(visitado[arco->getNodeDest() - 1] == 0){
            No *no_aux = g->getNoInicial();

            while(no_aux != NULL){
                if(no_aux->getId() == arco->getNodeDest())
                    break;
                no_aux = no_aux->getProx();
            }
            arco = arco->getProx();
            DFSGeral(no_aux, g, visitado);
        }
    }

    // for (No *no = noPartida; no != NULL; no = no->getProx())
    // {
    //     if(g->existeArco(noPartida->getId(),no->getId()) && visitado[no->getId()-1]  == 0)
    //         DFSGeral(no,g,visitado);
    // }
}

bool DFSCaminho(No *noPartida,No* Destino, Grafo *g,int* visitado){//o no de partida é iniciado como o no inicial do grafo
    visitado[noPartida->getId() - 1] = 1;
    for (No *no = noPartida; no != NULL; no = no->getProx())
    {
        if(no->getId() == Destino->getId())
            return true;
        if(g->existeArco(noPartida->getId(),no->getId()) && visitado[no->getId() - 1] == 0)
            DFSGeral(no,g,visitado);
    }
    return false;//usar para fecho transitivo direto e indireto
}

Grafo* criarCopia(Grafo *g){
    Grafo* copia = new Grafo(g->getOrdem(), g->ehDir(), g->ehPondAr(), g->ehPondNode());

    No* no = g->getNoInicial();

    while(no != NULL){
        copia->inserirNo(no->getId(), no->getPeso());
        no = no->getProx();
    }

    no = g->getNoInicial();

    while(no != NULL){
        Arco *arco = no->getAdjacentes();

        int *visitados = new int[g->getOrdem()];
                int v = 1;
                bool visitado = false;
                while(no != NULL){
                    Arco *arco = no->getAdjacentes();
                    visitados[v-1] = no->getId();
                    while(arco != NULL){
                        for(int i = 0; i < v; i++){
                            if(arco->getNodeDest() == visitados[i])
                                visitado = true;
                        }
                        if(visitado)
                            copia->inserirArco(no->getId(), arco->getNodeDest(), arco->getPeso());
                        arco = arco->getProx();
                        visitado = false;
                    }
                    no = no->getProx();
                    v++; 
                    delete arco;
                }
    }
    
    return copia;
}

bool ehNoArticulacao(Grafo *g, int noId){
    Grafo *g_aux = criarCopia(g);
    g_aux->removerNo(noId);
    int *visitado = new int[g_aux->getOrdem()];
    for(int i = 0; i < g_aux->getOrdem(); i++){
        visitado[i] = -1;
    }
    DFSGeral(g_aux->getNoInicial(), g_aux, visitado);
    for(int i = 0; i < g_aux->getOrdem(); i++){
        if(visitado[i] != 1)
        {
            cout << i+1 << endl;
        }
    }
    return false;


}

bool ehKRegular(Grafo *g, int k){
    int numArestas = k*g->getOrdem()/2;
    cout << "\tOrdem: "<<g->getOrdem()<<endl;
    cout << "\tK: "<<k<<endl;
    cout << "\tNúmero de arcos do grafo: "<<g->getNumArcos()<<endl;
    cout << "\tCalculado: "<<numArestas<<endl;

    if(numArestas != g->getNumArcos()){
        cout <<"\tGrafo não é "<<k<<"-regular"<<endl;
        return false;
    }

    No *aux = g->getNoInicial();
    while(aux!=NULL){
        if(aux->getGrauEntrada() != k) 
            return false;
        aux = aux->getProx();
    }

    cout << "\tGrafo regular. Verificou todo grafo e não encontrou nó com grau != "<<k<<endl;
    return true;
}

#endif