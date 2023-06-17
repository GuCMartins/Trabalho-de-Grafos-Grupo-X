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

bool Cluster::setSVertices(int* NodeOriDest, Grafo *grafoGeral)
{
    No* NoDest = grafoGeral->findNoById(NodeOriDest[1]);

    if(this->SVertices + NoDest->getPeso() <= this->max && NoDest->getEmCluster() == false)
    {
        this->SVertices += NoDest->getPeso();

        this->grafo->inserirNo(NodeOriDest[1], NoDest->getPeso());

        this->grafo->findNoById(NodeOriDest[1])->setEmCluster();

        Arco *arco = this->grafo->findNoById(NodeOriDest[0])->getAdjacente();

       while(arco != NULL){
            if(arco->getNodeDest() == NodeOriDest[1])//encontra o arco que liga o no fonte ao no a ser adicionado
                break;
            arco = arco->getProx();    
        }

        float pesoArco = arco->getPeso();

        this->grafo->inserirArco(NodeOriDest[0], NodeOriDest[1], pesoArco);
        
        setSArestas(pesoArco);

        if(this->SVertices >= this->min)
            this->dentroIntervalo = true;
        return true;
    }
    return false;
}

void DFSAnalisar_arestas(No *noPartida, Grafo *grafoGeral, int* visitado,float maiorPeso,int* NodeOriDest){//o no de partida é iniciado como o no inicial do grafo
    visitado[noPartida->getId()] = 1;
    Arco *arco = noPartida->getAdjacentes();

    while(arco != NULL){
        for(int i =0;i<noPartida->getGrauSaida();i++){
            if(arco->getPeso() > maiorPeso && noPartida->getEmCluster() == false && grafoGeral->findNoById(arco->getNodeDest())->getEmCluster() == false){
                maiorPeso = arco->getPeso();
                NodeOriDest[0] = noPartida->getId();
                NodeOriDest[1] = arco->getNodeDest();
            }
        }
        if(visitado[arco->getNodeDest() - 1] == -1)//se o no segguinte nao foi visitado
            DFSAnalisar_arestas(grafoGeral->findNoById(arco->getNodeDest()), grafoGeral, visitado,maiorPeso,NodeOriDest);
        arco = arco->getProx();
    }//a ideia é alterar o vetor de Nos ideias para retornar os que ainda nao estão ocupados
}

int* Cluster::melhorEscolha(Grafo* grafoGeral)//busca a aresta livre de maior peso para adicionar ao cluster, verificando se os nos que fazem parte dela
{                                             // ja estao no cluster e se a soma dos pesos de seus nos fazem com que o cluster esteja dentro do intervalo
    int *melhorEscolha = new int[2];
    DFSAnalisar_arestas(grafoGeral->getNoInicial(),grafoGeral,new int[grafoGeral->getOrdem()],0,melhorEscolha);//retorna os dois melhores nos para serem inseridos no cluster

    return melhorEscolha;//terminar a funcao
}

