#include "../include/Grafo.h"

#include <iostream>

using namespace std;

Grafo::Grafo(int n, bool direc, bool pondAresta, bool pondNode){}
Grafo::~Grafo(){}

Grafo::~Grafo()
{
}


int Grafo::getOrdem()
{
    return this->ordem;
}

int *Grafo:: vizinhancaAberta(int idNo){
    int size = 0;
    if(direcionado)
        size = Nos[idNo].getGrauSaida() + Nos[idNo].getGrauEntrada();
    else
        size = Nos[idNo].getGrauEntrada();    
    int *vizinhanca = new int[size];//aloca um vetor de vizinhanca com o numero de vizinhos do no
    int i = 0;
    Arco* arco = this->Nos[idNo].getAdjacente();//pega a aresta que sai do nó
    while(i<size && arco != NULL){
        if(arco->getNoDestino()->getId() == idNo || arco->getNoOrigem()->getId() == idNo){
            vizinhanca[i] = arco->getNoDestino()->getId();
            i++;
        }
        arco = arco->getProximo();
    }
    return vizinhanca;
}

int *Grafo::vizinhancaFechada(int idNo)
{
    int size = 0;
    if(direcionado)
        size = Nos[idNo].getGrauSaida() + Nos[idNo].getGrauEntrada();
    else
        size = Nos[idNo].getGrauEntrada();    
    int *vizinhanca = new int[size];//aloca um vetor de vizinhanca com o numero de vizinhos do no
    int *vizinhanca = new int[size+1];
    vizinhanca = vizinhancaAberta(idNo);//recebe a vizinhanca aberta e completa com o no que esta sendo analisado
    vizinhanca[size] = idNo;
    return vizinhanca;
}
