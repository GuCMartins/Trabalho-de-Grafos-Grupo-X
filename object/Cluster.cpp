#include "../include/Arco.h"
#include "../include/No.h"
#include "../include/Grafo.h"
#include "../include/Cluster.h"
#include <string>

#include <iostream>

using namespace std;

Cluster::Cluster(int min, int max, Grafo *grafo, std::string type)
{
    //Inicialização dos valores do cluster->
    this->min = min;
    this->max = max;
    this->sumVertices = 0;
    this->sumArestas = 0;
    this->numVertices = 0;
    this->dentroIntervalo = false;
    this->type = type;
    this->cluster_graph = new Grafo(grafo->getOrdem(), grafo->ehDir(), grafo->ehPondAr(), grafo->ehPondNode()); //Grafo que será induzido a paritr dos nós que serão adicionados no cluster
}

Cluster::~Cluster()
{
    
}

//Função auxiliar para verificar se aquela adjacência que está sendo avaliada é de um dos nós que já foram inseridos no grafo do cluster
bool Cluster::confereNosInseridos(int val){
    for(forward_list<int>::iterator it = inseridos.begin(); it != inseridos.end(); it++){
        if(*it == val){
            return true;
        }
    }
    return false;
}

void Cluster::inserirNoCluster(No* nodeToInsert){
    this->cluster_graph->inserirNo(nodeToInsert->getId(), nodeToInsert->getPeso());
    
    for(Arco *adjacentes = nodeToInsert->getAdjacentes(); adjacentes != NULL; adjacentes = adjacentes->getProx()){
        if(confereNosInseridos(adjacentes->getNodeDest())){
            sumArestas += adjacentes->getPeso();
            this->cluster_graph->inserirArco(nodeToInsert->getId(), adjacentes->getNodeDest(), adjacentes->getPeso());
        }
    }
    
    sumVertices += nodeToInsert->getPeso();
    if(sumVertices >= this->min && sumVertices <= this->max)
        this->dentroIntervalo = true;
    else
        this->dentroIntervalo = false;
    
    this->inseridos.push_front(nodeToInsert->getId());
    //! entender depois o problema grave relacionado a esse print
    // for(std::forward_list<int>::iterator it = this->inseridos.begin(); it != this->inseridos.end(); it++){
    //             cout << (*it) << " ";
    // }
    // cout << endl;
    // cout << "--------" << endl;
    this->numVertices++; 
}

// bool Cluster::setSumVertices(int idNodeFonte,int idNode,  float pesoNode)
// {
//     if(this->SVertices + pesoNode <= this->max)
//     {
//         this->SVertices += pesoNode;

//         this->grafo->inserirNo(idNode, pesoNode);

//         Arco *arco = this->grafo->findNoById(idNodeFonte)->getAdjacente();

//        while(arco != NULL){
//             if(arco->getNodeDest() == idNode)
//                 break;
//             arco = arco->getProx();    
//         }

//         float pesoArco = arco->getPeso();

//         this->grafo->inserirArco(idNodeFonte, idNode, pesoArco);
        
//         setSArestas(pesoArco);

//         if(this->SVertices >= this->min)
//             this->dentroIntervalo = true;
//         return true;
//     }
//     return false;
// }

// int* Cluster::melhorEscolha(Grafo* grafoGeral)
// {
//     int *melhorEscolha = new int[2];
//     melhorEscolha[0] = this->grafo->getNoInicial()->getId();

//     No *noFonteGeral = grafoGeral->findNoById(melhorEscolha[0]);

//     No *noGeral = noFonteGeral->getProx();

//     while(noGeral != NULL)//analisar os nos adjacentes a partir do inicial para encontrar um
//     {                //que tenha uma aresta com o no inicial com o maior peso

//         if(this->grafo->findNoById(noGeral->getId()) != NULL)//se o no ja estiver no cluster
//         {
//             noGeral = noGeral->getProx();
//         }
//     }

//     return melhorEscolha;//terminar a funcao
// }