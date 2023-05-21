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
}