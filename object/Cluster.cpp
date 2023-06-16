#include "../include/Arco.h"
#include "../include/No.h"
#include "../include/Grafo.h"
#include "../include/Cluster.h"

#include <iostream>

using namespace std;

Cluster::Cluster(int min, int max, Grafo *grafo)
{
    this->min = min;
    this->max = max;
    this->grafo = new Grafo(0, grafo->ehDir(), grafo->ehPondAr(), grafo->ehPondNode());//cria grafos menores com as configurações do grafo original
}

Cluster::~Cluster()
{
}

bool Cluster::setSVertices(int idNodeFonte,int idNode,  float pesoNode)
{
    if(this->SVertices + pesoNode <= this->max)
    {
        this->SVertices += pesoNode;

        this->grafo->inserirNo(idNode, pesoNode);

        Arco *arco = this->grafo->findNoById(idNodeFonte)->getAdjacente();

       while(arco != NULL){
            if(arco->getNodeDest() == idNode)
                break;
            arco = arco->getProx();    
        }

        float pesoArco = arco->getPeso();

        this->grafo->inserirArco(idNodeFonte, idNode, pesoArco);
        
        setSArestas(pesoArco);

        if(this->SVertices >= this->min)
            this->dentroIntervalo = true;
        return true;
    }
    return false;
}

int* Cluster::melhorEscolha(Grafo* grafoGeral)
{
    int *melhorEscolha = new int[2];
    melhorEscolha[0] = this->grafo->getNoInicial()->getId();

    No *noFonteGeral = grafoGeral->findNoById(melhorEscolha[0]);

    No *noGeral = noFonteGeral->getProx();

    while(noGeral != NULL)//analisar os nos adjacentes a partir do inicial para encontrar um
    {                //que tenha uma aresta com o no inicial com o maior peso

        if(this->grafo->findNoById(noGeral->getId()) != NULL)//se o no ja estiver no cluster
        {
            noGeral = noGeral->getProx();
        }
    }

    return melhorEscolha;//terminar a funcao
}

