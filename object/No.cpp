#include "../include/No.h"

#include <iostream>

using namespace std;

No::No(int id, float peso)
{
    this->id = id;
    this->peso = peso;
    this->grauEntrada = 0;
    this->grauSaida = 0;
    this->proximo = NULL;
    this->adjacentes = NULL;
}

No::~No()
{
    // Ao excluir um nó, deletar da memória todos seus adjacentes
    Arco *aux = adjacentes;
    while (adjacentes != NULL)
    {
        aux = adjacentes->getProx();
        //cout << "DELETANDO " << adjacentes->getNodeDest() << endl;
        delete adjacentes;
        adjacentes = aux;
    }
}

int No::getX()
{
    return this->x;
}

int No::getY()
{
    return this->y;
}

float No::getPeso()
{
    return this->peso;
}