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
        //Ao excluir um nó, deletar da memória todos seus adjacentes
        Arco *aux = adjacentes;
        while (adjacentes != NULL)
        {
            aux = adjacentes->getProx();
            cout <<"DELETANDO "<<adjacentes->getNodeDest()<<endl;
            delete adjacentes;
            adjacentes = aux;
        }
}

Arco* No::existeNoAdjacente(int idNo){
    Arco *aux = adjacentes;
    while (aux != NULL)
    {
        if(aux->getNodeDest() == idNo){
            return aux;
        }
            
        aux = aux->getProx();
    }

    return NULL;
}