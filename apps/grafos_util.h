#ifndef GRAFOSUTIL_CPP
#define GRAFOSUTIL_CPP
#include <limits>
#include <algorithm>
#include <iostream>
#include <queue>
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

//src é o nó que está sendo considerado para calcular o caminho mínimo
float* dijkstraAlgorithm(Grafo *g, int src){
    
    bool* visited = new bool[g->getOrdem()];
    float* distancias = new float[g->getOrdem()];
    
    //Cria uma min-heap
    priority_queue<pair<float,int>, vector<pair<float,int>>, greater<pair<float,int>>> pq;
    
    for(int i = 0; i < g->getOrdem(); i++){
        distancias[i] = numeric_limits<int>::max();
        visited[i] = false; //False significa que não foi visitado, e true o contrário.
    }

    distancias[src-1] = 0;
    pq.push(make_pair(0, src-1));

    while(!pq.empty()){
        int id = pq.top().second;
        pq.pop();

        if(visited[id] == false){
            visited[id] = true;

            No *node = g->findNoById(id + 1);
            for (Arco *aux = node->getAdjacentes(); aux != NULL; aux = aux->getProx())
            {
                int x = aux->getNodeDest()-1;
                float custo = aux->getPeso();

                if (distancias[x] > (distancias[id] + custo))
                {
                    distancias[x] = distancias[id] + custo;
                    pq.push(make_pair(distancias[x], x));
                }
            }
        }
    }

    return distancias;
}

/* Parte do Algoritmo de Floyd-Warshal */

void floydWarshalAlgorithm(Grafo* g){
    float** dist = new float*[g->getOrdem()];

    for(int i = 0; i < g->getOrdem(); i++){
        dist[i] = new float[g->getOrdem()];
        for(int j = 0; j < g->getOrdem(); j++){
            dist[i][j] = numeric_limits<int>::max();
        }
    }

    int i = 0;
    for(No *node = g->getNoInicial(); node != NULL; node = node->getProx()){
        Arco* edge = node->getAdjacentes();
        for(int j = 0; j < g->getOrdem(); j++){
            if(i == j){
                dist[i][j] = 0;
            }else if(edge != NULL){
                if(j == edge->getNodeDest()-1){
                    dist[i][edge->getNodeDest()-1] = edge->getPeso();
                    edge = edge->getProx();
                }
            }
            
            // cout << "Valor de i: " << i << "Valor de j: " << j << endl;
            cout << dist[i][j] << " ";
        }
        i++;
        cout << endl;
    } 
}

/* Parte da Busca em Profundidade */

void DFSGeral(No *noPartida, Grafo *g, int* visitado){//o no de partida é iniciado como o no inicial do grafo
    visitado[noPartida->getId() - 1] = 1;
    Arco *arco = noPartida->getAdjacentes();

    while(arco != NULL){
        if(visitado[arco->getNodeDest() - 1] == -1)
            DFSGeral(g->findNoById(arco->getNodeDest()), g, visitado);
        arco = arco->getProx();
    }
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

//Estrutura para auxiliar na hora de construir a árvore geradora mínima.
typedef struct{
    int v1, v2;
    float peso;
} ArcoUtils;

int find(int* parents, int i){
    if(parents[i] == i)
        return i;
    return find(parents, parents[i]);
}

void Union(int* parents, int x, int y){
    parents[x] = y;
}

//x--> controla o número de arestas no edges
bool isCycle(Grafo* g, ArcoUtils* edges, int k){
    int* parents = new int[g->getOrdem()];

    for(int i = 0; i < g->getOrdem(); i++)
        parents[i] = i;

    for(int i  = 0; i < k; i++){
        int x = find(parents, edges[i].v1-1);
        int y = find(parents, edges[i].v2-1);
        if(x == y)
            return true;

        Union(parents, x, y);
    }
    return false;
}

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

    ArcoUtils* edgesTree = new ArcoUtils[g->getOrdem() - 1];
    //Crio um grafo vazio, esse grafo terá no máximo V-1 arestas.
    Grafo *gAux = new Grafo(g->getOrdem(), g->ehDirecionado(), g->ehPondAr(),g->ehPondNode());

    //Insiro os nós no grafo  
    for (int i = 0; i < g->getOrdem(); i++)
        gAux->inserirNo(i + 1, 0);

    for(int i = 0; i < g->getOrdem()-1; i++){
        while(c < g->getNumArcos()){
            
            //Insiro uma aresta nessa lista de arestas
            edgesTree[i].v1 = edgesHelper[c].v1; 
            edgesTree[i].v2 = edgesHelper[c].v2;
            edgesTree[i].peso = edgesHelper[c].peso;
            
            if(isCycle(gAux, edgesTree, i+1)){
                //Forma ciclo, essa aresta de menor peso não pode ser inserida
                c++;
            }else{
                //Não formou ciclo, insiro ela no grafo, e verifico as próximas
                gAux->inserirArco(edgesHelper[c].v1, edgesHelper[c].v2, edgesHelper[c].peso);
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
    int *visitado = new int[g->getOrdem()];
    for(int i = 0; i < g->getOrdem(); i++){
        visitado[i] = -1;
    }
    DFSGeral(g_aux->getNoInicial(), g_aux, visitado);
    for(int i = 0; i < g->getOrdem(); i++){
        if(visitado[i] != 1 && i != noId - 1)
            return true;
    }
    return false;
}

bool ehArestaPonte(Grafo *g, int idNodeOrig, int idNodeDest){
    Grafo*g_aux = criarCopia(g);
    g_aux->removerArco(idNodeOrig, idNodeDest);
    int *visitado = new int[g->getOrdem()];
    for(int i = 0; i < g->getOrdem(); i++){
        visitado[i] = -1;
    }
    DFSGeral(g_aux->getNoInicial(), g_aux, visitado);
    for(int i = 0; i < g->getOrdem(); i++){
        if(visitado[i] != 1)
            return true;
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
            cout << "O no " << no->getId() << " eh de articulacao" << endl;
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