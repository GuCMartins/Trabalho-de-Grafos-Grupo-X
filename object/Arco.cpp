#include "../include/Arco.h"
#include "../include/No.h"

#include <iostream>

using namespace std;

Arco::Arco(int peso, Arco *prox, No *origem, No *destino)
{
    pesoArco = peso;
    proxArco = prox;
    noOrigem = origem;
    noDestino = destino;
}

Arco::~Arco()
{
}

int Arco::getPeso()
{
    return pesoArco;
}

int Arco::getNoOrigem()
{
    return noOrigem->getId();
}

int Arco::getNoDestino()
{
    return noDestino->getId();
}