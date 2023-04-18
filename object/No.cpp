#include "../include/No.h"

#include <iostream>

using namespace std;

No::No(int id, int x, int y, float peso)
{
    this->id = id;
    this->x = x;
    this->y = y;
    this->peso = peso;
    this->visitado = false;
    this->grauEntrada = 0;
    this->grauSaida = 0;
    this->proximo = NULL;
    this->adjacente = NULL;
}

No::~No()
{
}