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

bool Grafo::Euleriano(){
//Teorema: Um multigrafo M é euleriano se e somente se M é conexo e cada vértice de M tem grau par.
    
    if(ordem == 0)
        return false;//se o grafo nao tiver nos, nao eh euleriano

    for(int i=0;i<ordem;i++){
        if(Nos[i].getGrauEntrada() <= 1)
            return false;//se algum no nao tiver arcos conectando outros dois nos (na saida ou na entrada), o grafo nao eh euleriano
        if(Nos[i].getGrauEntrada()%2!=0 || Nos[i].getGrauSaida()%2 !=0)
            return false;//se algum no tiver grau de entrada diferente do grau de saida, o grafo nao eh euleriano   
    }
    return true;
}


