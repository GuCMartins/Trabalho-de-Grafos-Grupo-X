#include "../include/Arco.h"

#include <iostream>

using namespace std;

Arco::Arco(int nodeDest, float w)
{
    this->nodeDest = nodeDest;
    this->w = w;
}

Arco::~Arco()
{
}

float Arco::getPeso(){
    return this->w;
}