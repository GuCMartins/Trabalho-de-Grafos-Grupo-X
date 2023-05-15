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
    int *vizinhanca = new int[Nos[idNo].getGrauSaida()];
    int i = 0;
    Arco* arco = this->Nos[idNo].getAdjacente();//pega a aresta que sai do nó
    while(i<Nos[idNo].getGrauSaida()){
        vizinhanca[i] = arco->getNoDestino()->getId();
        arco = arco->getProximo();
        i++;
    }
    return vizinhanca;
}

int *Grafo::vizinhancaFechada(int idNo)
{
    int *vizinhanca = new int[Nos[idNo].getGrauSaida()+1];
    vizinhanca = vizinhancaAberta(idNo);
    vizinhanca[Nos[idNo].getGrauSaida()] = idNo;
    return vizinhanca;
}
