#include "../include/Grafo.h"

#include <iostream>

using namespace std;

Grafo::Grafo(int ordem, bool direc, bool pondAresta, bool pondNode)
{
    this->ordem = ordem;
    this->direcionado = direc;
    this->pondAresta = pondAresta;
    this->pondNode = pondNode;
    this->noInicial = NULL;
    this->numArcos = 0;
}
Grafo::~Grafo() {}

void Grafo::inserirNo(int idNode, float pesoNode)
{
    No *novoNo = new No(idNode, pesoNode);

    if (noInicial == NULL)
    {
        this->noInicial = novoNo;
    }
    else
    {
        No *noInsercao = noInicial;
        while (noInsercao->getProx() != NULL)
        {
            noInsercao = noInsercao->getProx();
        }

        noInsercao->setProx(novoNo);
    }
}

void Grafo::inserirArco(int idNoOrigem, int idNoDestino, float pesoArco)
{
    No *noOrigem = this->findNoById(idNoOrigem);
    No *noDestino = this->findNoById(idNoDestino);

    if (noOrigem != NULL && noDestino != NULL)
    {
        if (this->direcionado)
        {
            this->auxInserirArco(noOrigem, noDestino, pesoArco);
            this->numArcos += 1;

            noDestino->incrementaGrauEntrada(1);
            noOrigem->incrementaGrauSaida(1);
        }
        else
        {
            noOrigem->incrementaGrauEntrada(1);
            noDestino->incrementaGrauEntrada(1);
            this->auxInserirArco(noOrigem, noDestino, pesoArco);
            this->auxInserirArco(noDestino, noOrigem, pesoArco);
            this->numArcos += 2;
        }
    }
    else
    {
        cout << "No origem/destino inexistente no grafo!" << endl;
    }
}

void Grafo::auxInserirArco(No *noOrigem, No *noDestino, float pesoArco)
{
    Arco *novoArco = new Arco(noDestino->getId(), pesoArco);

    Arco *arcoInsercao = noOrigem->getArcoAdjacente();

    if (arcoInsercao == NULL)
    {
        noOrigem->setArcoAdjacente(novoArco);
    }
    else
    {
        while (arcoInsercao->getProx() != NULL)
        {
            arcoInsercao = arcoInsercao->getProx();
        }
        arcoInsercao->setProx(novoArco);
    }
}

void Grafo::removerNo(int idNode)
{
    No *busca = findNoById(idNode);
    if (busca == NULL)
    {
        cout << "O nó " << idNode << " não existe no grafo e não pode ser removido.." << endl;
        return;
    }

    Arco *arcoInicial = busca->getArcoAdjacente();
    if (arcoInicial == NULL)
    {
        cout << "Grafo não tem arcos, removendo só o nó " << idNode << endl;
        delete busca;
    }

    Arco *aux;
    while (arcoInicial->getProx() != NULL)
    {
        aux = arcoInicial->getProx();
        delete arcoInicial;
        arcoInicial = aux;
    }

    delete busca;
}

void Grafo::removerArco(int idNoOrigem, int idNoDestino)
{
    int a = 0;
}

No *Grafo::findNoById(int id)
{
    No *no = noInicial;
    while (no != NULL)
    {
        if (no->getId() == id)
        {
            return no;
        }
        no = no->getProx();
    }

    return NULL;
}

void Grafo::imprimirListaNos()
{
    No *no = noInicial;
    while (no != NULL)
    {
        cout << "ID: " << no->getId() << " PESO: " << no->getPeso() << endl;
        no = no->getProx();
    }
}

void Grafo::imprimirListaNosAdjacentes(int idNo)
{
    No *no = findNoById(idNo);

    if (no == NULL)
    {
        cout << "Nó com id " << idNo << " não encontrado no grafo..." << endl;
        return;
    }
    Arco *arco = no->getArcoAdjacente();
    if (arco == NULL)
    {
        cout << "Nó com id " << idNo << " não tem adjacentes" << endl;
        return;
    }
    cout << "Nós adjacentes ao nó: " << idNo << endl;
    while (arco != NULL)
    {
        cout << "ARCO NO ID: " << arco->getNodeDest() << endl;
        arco = arco->getProx();
    }
}