#include "../include/Arco.h"

#include <iostream>

using namespace std;

Arco::Arco(int nodeDest, float w)
{
    this->nodeDest = nodeDest;
    this->w = w;
    this->proxArco = NULL;
}

Arco::~Arco()
{
}

float Arco::getPeso(){
    return this->w;
}