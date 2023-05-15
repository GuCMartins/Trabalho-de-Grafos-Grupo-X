#include "../include/Arco.h"
#include "../include/No.h"

#include <iostream>

using namespace std;

Arco::Arco(float peso, Arco *prox, No *origem, No *destino)
{
    pesoArco = peso;
    proxArco = prox;
    noOrigem = origem;
    noDestino = destino;
}

Arco::~Arco()
{
}

float Arco::getPeso()
{
    return pesoArco;
}
