#ifndef GRAFOSUTIL_CPP
#define GRAFOSUTIL_CPP
#include <limits>
#include <algorithm>
#include <iostream>
#include "../include/Grafo.h"
#include <forward_list>

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

/*Verificação de Ciclos no grafo && Árvore Geradora Mínima(Algoritmo de Kruskall)*/

bool isCycleAux(Grafo *g, int no, int parent, bool* visit){
    visit[no] = true;
    
    No *node = g->findNoById(g->getOrdem() - no);
    cout << "Valor do no: " << no << ", Valor id do node: " << node->getId() << endl;

    for(Arco *aux = node->getAdjacentes(); aux != NULL; aux = aux->getProx()){
        if(!visit[g->getOrdem()-aux->getNodeDest()]){
            if(isCycleAux(g, g->getOrdem() - aux->getNodeDest(), no, visit) == true)
                return true;
        }else if(g->getOrdem() - aux->getNodeDest() != parent){
            return true;
        }
    } 
    return false;
}

bool isCycle(Grafo *g){
    
    //Marco todos os vértices como não visitados  
    bool *visit = new bool[g->getOrdem()];
    for(int i = 0; i < g->getOrdem(); i++){
        visit[i] = false;
    }
    
    //Chamada recursiva da função para ajudar a verificar se tem ciclo
    for(int i = 0; i < g->getOrdem(); i++){
        if(!visit[i]){
            if(isCycleAux(g, i, -1, visit) == true){
                return true;
            }
        }
    }
    return false;
}

//Estrutura para auxiliar na hora de construir a árvore geradora mínima.
typedef struct{
    int v1, v2;
    float peso;
} ArcoUtils;

void swap(ArcoUtils arr[], int i, int j){
    ArcoUtils aux;
    aux.v1 = arr[i].v1;
    aux.v2 = arr[i].v2;
    aux.peso = arr[i].peso;
    arr[i] = arr[j];
    arr[j] = aux;
}

int partition(ArcoUtils arr[], int low, int high)
{
    // Choosing the pivot
    int pivot = arr[high].peso;
 
    // Index of smaller element and indicates
    // the right position of pivot found so far
    int i = (low - 1);
 
    for (int j = low; j <= high - 1; j++) {
 
        // If current element is smaller than the pivot
        if (arr[j].peso < pivot) {
 
            // Increment index of smaller element
            i++;
            swap(arr, i, j);
        }
    }
    swap(arr, i + 1, high);
    return (i + 1);
}
 
// The main function that implements QuickSort
// arr[] --> Array to be sorted,
// low --> Starting index,
// high --> Ending index
void quickSort(ArcoUtils arr[], int low, int high)
{
    if (low < high) {
 
        // pi is partitioning index, arr[p]
        // is now at right place
        int pi = partition(arr, low, high);
 
        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

Grafo* kruskalAlgorithm(Grafo *g){
    //Pré-requisito -> Criar uma lista com as arestas ordenadas em ordem crescente de pesos.
    
    int **verifyMatrix = new int*[g->getOrdem()];
    
    for(int i = 0; i < g->getOrdem(); i++){
        verifyMatrix[i] = new int[g->getOrdem()];
        for(int j = 0; j < g->getOrdem(); j++){
            verifyMatrix[i][j] = -1;
        }
    }
    ArcoUtils* edgesHelper = new ArcoUtils[g->getNumArcos()];
    int count = 0;
    for(No* node = g->getNoInicial(); node != NULL; node = node->getProx()){
        for(Arco* edge = node->getAdjacentes(); edge != NULL; edge = edge->getProx()){
            if(verifyMatrix[node->getId()-1][edge->getNodeDest()-1] == -1 && verifyMatrix[edge->getNodeDest()-1][node->getId()-1] == -1){
                verifyMatrix[node->getId()-1][edge->getNodeDest()-1] = 1;
                edgesHelper[count].v1 = node->getId();
                edgesHelper[count].v2 = edge->getNodeDest();
                edgesHelper[count].peso = edge->getPeso();
                count++;
            }
        }
    }
    
    quickSort(edgesHelper, 0, g->getNumArcos()-1);

    // cout << "-----------------------------------" << endl;
    // for(int i = 0; i < g->getNumArcos(); i++){
    //     cout << edgesHelper[i].v1 << " " << edgesHelper[i].v2<< " " << edgesHelper[i].peso << " " << endl;
    // }

    //Parte para montar a árvore geradora mínima
    int c = 0;
    Grafo *gAux = new Grafo(g->getOrdem(), g->ehDirecionado(), g->ehPondAr(),g->ehPondNode());

    for(int i = 0; i < g->getOrdem()-1; i++){
        while(c < g->getNumArcos()){
            
            gAux->inserirNo(edgesHelper[c].v1, 0);
            gAux->inserirNo(edgesHelper[c].v2, 0);
            gAux->inserirArco(edgesHelper[c].v1, edgesHelper[c].v2, edgesHelper[c].peso);
            cout << "Vertices: " << edgesHelper[c].v1 << " , " << edgesHelper[c].v2 << endl;
            if(isCycle(gAux)){
                gAux->removerNo(edgesHelper[c].v1);
                gAux->removerNo(edgesHelper[c].v2);
                gAux->removerArco(edgesHelper[c].v1, edgesHelper[c].v2);
                c++;
            }else{
                c++;
                break;
            }
        }
    }

    return gAux;
}

/*Verificação de Nó Articulação*/

Grafo* criarCopia(Grafo *g){
    Grafo* copia = new Grafo(g->getOrdem(), g->ehDir(), g->ehPondAr(), g->ehPondNode());

    No* no = g->getNoInicial();

    while(no != NULL){
        copia->inserirNo(no->getId(), no->getPeso());
        no = no->getProx();
    }

    no = g->getNoInicial();

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

bool ehArestaPonte(Grafo *g, int idNodeOrig, int idNodeDest){
    Grafo*g_aux = criarCopia(g);
    g_aux->removerArco(idNodeOrig, idNodeDest);
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

void nosArticulacao(Grafo *g){
    No* no = g->getNoInicial();
    
    int i = 0;
    cout << "Nos de articulacao: " << endl;

    while(no != NULL){
        if(ehNoArticulacao(g, no->getId()))
        {
            cout << no->getId() << endl;
            i++;
        }
        no = no->getProx();
    }

    if(i == 0)
        cout << "nenhum no do grafo eh de articulacao." << endl;
}

void arestasPonte(Grafo *g){
    No* no = g->getNoInicial();
    int cont = 0;
    cout << "Arestas ponte: " << endl;

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
                if(ehArestaPonte(g, no->getId(), arco->getNodeDest())){
                    cont++;
                    cout << no->getId() << " e " << arco->getNodeDest() << endl;} 
            arco = arco->getProx();
            visitado = false;
        }
        no = no->getProx();
        v++; 
        delete arco;
    }

    if(cont == 0)
    cout << "Nao existe aresta ponte no grafo." << endl;
}


//TODO: retornar o nó, vetor de int ou imprimir o GE/GS dentro da função???
No* getGrauNo(Grafo *g, int idNode){
    No* no = g->findNoById(idNode);
    return no;
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

Grafo* subgrafoInduzido(Grafo *grafo, int *idNos, int *size){
    //1. Inserir os nós no subrafoInduzido que existem no grafo original
    Grafo *subGrafoInduzido = new Grafo(*size, grafo->ehDir(), grafo->ehPondAr(), grafo->ehPondNode());
    forward_list<int> idNosInseridos;
    int countNosInseridos = 0;

    for (int i = 0; i < *size; i++) {
        No *no = grafo->findNoById(idNos[i]);
        if(no != NULL){
            subGrafoInduzido->inserirNo(no->getId(),no->getPeso());
            idNosInseridos.push_front(no->getId());
            countNosInseridos++;
        }
    }

    subGrafoInduzido->setOrdem(countNosInseridos);

    //2. Para cada nó k do subGrafoInduzido, encontro o nó k no grafo original e verifico se existe entre os adjacentes
    No *noAuxSubGrafoInduzido = subGrafoInduzido->getNoInicial();
    No *noAuxGrafoOriginal;

    while(noAuxSubGrafoInduzido != NULL){
        //procuro no grafo original o nó k da da iteração atual
        noAuxGrafoOriginal = grafo->findNoById(noAuxSubGrafoInduzido->getId());
        
        // procuro nos adjacentes do nó k se existe algum dos id do subgrafo induzido
        for(forward_list<int>::iterator it = idNosInseridos.begin();it != idNosInseridos.end();it++){
            if(*it != noAuxGrafoOriginal->getId()){
                //verificando se existe o idNode
                Arco *auxArco = noAuxGrafoOriginal->existeNoAdjacente(*it);
                
                if(auxArco != NULL){
                    subGrafoInduzido->inserirArco(noAuxSubGrafoInduzido->getId(), *it, auxArco->getPeso());
                }
            }
            
        }
        noAuxSubGrafoInduzido = noAuxSubGrafoInduzido->getProx();
    }
    

    return subGrafoInduzido;

}

Grafo *grafoComplementar(Grafo *grafo){
    Grafo *novoGrafo = new Grafo(grafo->getOrdem(), grafo->ehDir(), grafo->ehPondAr(), grafo->ehPondNode());
    No *aux1 = grafo->getNoInicial();
    
    while(aux1 != NULL){
        novoGrafo->inserirNo(aux1->getId(), aux1->getPeso());
        aux1 = aux1->getProx();
    }

    No *aux2;
    aux1 = grafo->getNoInicial(); 
    while(aux1 != NULL){
        aux2 = grafo->getNoInicial();
        while(aux2 != NULL){
            if(aux1->getId() != aux2->getId()){
                //cout <<"Verificando se existe no nó "<<aux1->getId()<<" o adjacente "<<aux2->getId()<<endl;
                if(aux1->existeNoAdjacente(aux2->getId()) == NULL){
                    novoGrafo->inserirArco(aux1->getId(), aux2->getId(), 1);
                }
            }
            
            aux2 = aux2->getProx();
        }
        aux1 = aux1->getProx();     
    }

    return novoGrafo;
}

#endif