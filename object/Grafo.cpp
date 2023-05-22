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
        if (this->ehDir())
        {
            this->auxInserirArco(noOrigem, noDestino, pesoArco);

            noDestino->incrementaGrauEntrada(1);
            noOrigem->incrementaGrauSaida(1);
        }
        else
        {
            noOrigem->incrementaGrauEntrada(1);
            noDestino->incrementaGrauEntrada(1);
            this->auxInserirArco(noOrigem, noDestino, pesoArco);
            this->auxInserirArco(noDestino, noOrigem, pesoArco);
        }
        this->numArcos += 1;
    }
    else
    {
        cout << "No origem/destino inexistente no grafo!" << endl;
    }
}

void Grafo::auxInserirArco(No *noOrigem, No *noDestino, float pesoArco)
{
    Arco *novoArco = new Arco(noDestino->getId(), pesoArco);

    Arco *arcoInsercao = noOrigem->getArcoAdjacentes();

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
    No *busca = noInicial;
    No *predecessor = NULL;

    while (busca != NULL && busca->getId() != idNode)
    {
        predecessor = busca;
        busca = busca->getProx();
    }

    if (busca->getId() != idNode)
    {
        cout << "O nó " << idNode << " não existe no grafo e não pode ser removido..." << endl;
        return;
    }

    Arco *arcoInicial = busca->getArcoAdjacentes();
    if (arcoInicial == NULL)
    {
        cout << "Nó não tem arcos, removendo só o nó " << idNode << endl;
        if (predecessor != NULL)
        {
            predecessor->setProx(busca->getProx());
        }
        delete busca;
        return;
    }

    Arco *aux = arcoInicial;
    while (arcoInicial->getProx() != NULL)
    {
        aux = arcoInicial->getProx();
        delete arcoInicial;
        arcoInicial = aux;
    }

    delete arcoInicial;

    // Se estou removendo o primeiro nó
    if (busca->getId() == noInicial->getId())
    {
        noInicial = busca->getProx();
    }
    else
    {
        predecessor->setProx(busca->getProx());
    }

    delete busca;
}

void Grafo::removerArco(int idNoOrigem, int idNoDestino)
{
    No *noOrigem = this->findNoById(idNoOrigem);
    No *noDestino = this->findNoById(idNoDestino);

    if (this->ehDir())
    {
        auxRemoverArco(noOrigem, noDestino);
        noDestino->decrementaGrauEntrada(1);
        noOrigem->decrementaGrauSaida(1);
    }
    else
    {
        auxRemoverArco(noOrigem, noDestino);
        auxRemoverArco(noDestino, noOrigem);
        noOrigem->decrementaGrauEntrada(1);
        noDestino->decrementaGrauEntrada(1);
    }
    this->numArcos -= 1;
}

void Grafo::auxRemoverArco(No *noOrigem, No *noDestino)
{

    Arco *busca = noOrigem->getArcoAdjacentes();
    if (busca == NULL)
    {
        cout << "Nó " << noOrigem->getId() << " não tem adjacente..." << endl;
        return;
    }

    Arco *predecessor = NULL;

    while (busca != NULL && busca->getNodeDest() != noDestino->getId())
    {
        predecessor = busca;
        busca = busca->getProx();
    }

    if (busca->getNodeDest() != noDestino->getId())
    {
        cout << "O arco " << noDestino->getId() << " não existe como adjacente do no " << noOrigem->getId() << endl;
        return;
    }

    if (predecessor == NULL)
    {
        cout << "Removendo primeiro adjacente..." << endl;
        noOrigem->setArcoAdjacente(busca->getProx());
        delete busca;
        return;
    }

    predecessor->setProx(busca->getProx());
    delete busca;
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
        cout << "ID: " << no->getId() << " PESO: " << no->getPeso() << " Grau Entrada: " << no->getGrauEntrada() << " Grau Saída : " << no->getGrauSaida() << endl;
        no = no->getProx();
    }
}

void Grafo::imprimirTodosNosAdjacentes()
{
    No *aux = noInicial;
    if (aux != NULL)
    {
        while (aux != NULL)
        {
            imprimirListaNosAdjacentes(aux->getId());
            aux = aux->getProx();
        }
    }
    else
    {
        cout << "Grafo não tem nó inicial..." << endl;
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
    Arco *arco = no->getArcoAdjacentes();
    if (arco == NULL)
    {
        cout << "Nó com id " << idNo << " não tem adjacentes" << endl;
        return;
    }
    cout << "Nós adjacentes ao nó: " << idNo << endl;
    while (arco != NULL)
    {
        cout << "NO DESTINO ID: " << arco->getNodeDest() << endl;
        arco = arco->getProx();
    }
}

int Grafo::getOrdem()
{
    return this->ordem;
}

int *Grafo:: vizinhancaAberta(int idNo){
    int size = 0;
    if(direcionado)
        size = Nos[idNo].getGrauSaida() + Nos[idNo].getGrauEntrada();
    else
        size = Nos[idNo].getGrauEntrada();    
    int *vizinhanca = new int[size];//aloca um vetor de vizinhanca com o numero de vizinhos do no
    int i = 0;
    Arco* arco = this->Nos[idNo].getAdjacente();//pega a aresta que sai do nó
    while(i<size && arco != NULL){
        if(arco->getNoDestino()->getId() == idNo || arco->getNoOrigem()->getId() == idNo){
            vizinhanca[i] = arco->getNoDestino()->getId();
            i++;
        }
        arco = arco->getProx();
    }
    return vizinhanca;
}

int *Grafo::vizinhancaFechada(int idNo)
{
    int size = 0;
    if(direcionado)
        size = Nos[idNo].getGrauSaida() + Nos[idNo].getGrauEntrada();
    else
        size = Nos[idNo].getGrauEntrada();    
    int *vizinhanca = new int[size];//aloca um vetor de vizinhanca com o numero de vizinhos do no
    int *vizinhanca = new int[size+1];
    vizinhanca = vizinhancaAberta(idNo);//recebe a vizinhanca aberta e completa com o no que esta sendo analisado
    vizinhanca[size] = idNo;
    return vizinhanca;
}

bool Grafo::Euleriano(){
//Teorema: Um multigrafo M é euleriano se e somente se M é conexo e cada vértice de M tem grau par.
    
    if(ordem == 0)
        return false;//se o grafo nao tiver nos, nao eh euleriano

    for(int i=0;i<ordem;i++){
        if(Nos[i].getGrauEntrada() <= 1)
            return false;//se algum no nao tiver arcos conectando outros dois nos (na saida ou na entrada), o grafo nao eh euleriano
        if(Nos[i].getGrauEntrada()%2!=0 || Nos[i].getGrauSaida()%2 !=0)
            return false;//se algum no tiver grau de entrada diferente do grau de saida, o grafo nao eh euleriano   
    }
    return true;
}
