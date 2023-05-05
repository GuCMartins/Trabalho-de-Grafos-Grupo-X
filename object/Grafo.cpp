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

int *Grafo::vizinhancaAberta(int idNo)
{
    int *vizinhanca = new int[this->ordem];
    int i = 0;
    for (int j = 0; j < this->ordem; j++)
    {
        if (Nos[j].getProximo().getId() == idNo)
        {
            vizinhanca[i] = Nos[j].getProximo().getId();
            i++;
        }
    }
    return vizinhanca;
}

int *Grafo::vizinhancaFechada(int idNo)
{
    int *vizinhanca = new int[this->ordem+1];
    vizinhanca = vizinhancaAberta(idNo);
    vizinhanca[this->ordem] = idNo;
    return vizinhanca;
}
