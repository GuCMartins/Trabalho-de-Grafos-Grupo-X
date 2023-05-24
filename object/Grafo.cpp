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

// void Grafo::removerNo(int idNode)
// {
//     No *busca = noInicial;
//     No *predecessor = NULL;

//     while (busca != NULL && busca->getId() != idNode)
//     {
//         predecessor = busca;
//         busca = busca->getProx();
//     }

//     if (busca->getId() != idNode)
//     {
//         cout << "O nó " << idNode << " não existe no grafo e não pode ser removido..." << endl;
//         return;
//     }

//     Arco *arcoInicial = busca->getArcoAdjacentes();
//     if (arcoInicial == NULL)
//     {
//         cout << "Nó não tem arcos, removendo só o nó " << idNode << endl;
//         if (predecessor != NULL)
//         {
//             predecessor->setProx(busca->getProx());
//         }
//         delete busca;
//         return;
//     }

//     Arco *aux = arcoInicial;
//     while (arcoInicial->getProx() != NULL)
//     {
//         aux = arcoInicial->getProx();
//         delete arcoInicial;
//         arcoInicial = aux;
//     }

//     delete arcoInicial;

//     // Se estou removendo o primeiro nó
//     if (busca->getId() == noInicial->getId())
//     {
//         noInicial = busca->getProx();
//     }
//     else
//     {
//         predecessor->setProx(busca->getProx());
//     }

//     delete busca;
// }

void Grafo::removerNo(int idNode)
{
    // Encontrar o Nó a ser removido
    // Para cada no adjacente procurar o nó e remover o correspondente nas adjacencias
    // remover o no
    
    No *noRemover = noInicial;
    No *predecessor = NULL;

    // Procuro na lista de nós, o nó a ser removido
    while (noRemover != NULL && noRemover->getId() != idNode)
    {
        predecessor = noRemover;
        noRemover = noRemover->getProx();
    }

    //se não encontrar, retorna; 
    if (noRemover == NULL)
    {
        cout << "O nó " << idNode << " não existe no grafo e não pode ser removido..." << endl;
        return;
    }

    Arco *adjacentes = noRemover->getArcoAdjacentes();

    //se a lista de adjacentes é NULL, o nó não tem adjacentes
    // removo o nó e retorno
    if (adjacentes == NULL)
    {
        cout << "Nó não tem arcos, removendo só o nó " << idNode << endl;
        
        //removendo do meio da lista, então atualizo o proximo do nó anterior
        if (predecessor != NULL)
        {
            predecessor->setProx(noRemover->getProx());
        }
        delete noRemover;
        return;
    }

    if(!this->ehDir()){
        // removo todas as ocorrências do nó nas outras adjacências
        No *aux;
        while(adjacentes!=NULL){
            cout <<"Removendo ocorrencias do  "<<idNode<<" no nó"<<adjacentes->getNodeDest()<<endl;
            
            aux = findNoById(adjacentes->getNodeDest());
            auxRemoverArco(aux, idNode);
            aux->decrementaGrauEntrada(1);

            adjacentes = adjacentes->getProx();
        }
    }else{
        //TODO: decrementar grau de entrada dos nós adjacentes removidos
        No *auxNo ;
        Arco *auxAdj = adjacentes;
        while (adjacentes != NULL)
        {
            auxNo = findNoById(adjacentes->getNodeDest());
            auxNo->decrementaGrauEntrada(1);
            
            auxAdj = adjacentes->getProx();
            cout <<"DELETANDO "<<adjacentes->getNodeDest()<<endl;
            delete adjacentes;
            adjacentes = auxAdj;
        }
    }

    noRemover->setArcoAdjacente(NULL);

    if(noRemover->getId() == noInicial->getId()){
            noInicial = noRemover->getProx();
    }else{
        predecessor->setProx(noRemover->getProx());
    }
    cout <<" Removendo nó "<<noRemover->getId()<<endl;
    delete noRemover;
}

void Grafo::removerArco(int idNoOrigem, int idNoDestino)
{
    No *noOrigem = this->findNoById(idNoOrigem);
    No *noDestino = this->findNoById(idNoDestino);

    if (this->ehDir())
    {
        auxRemoverArco(noOrigem, idNoDestino);
        noDestino->decrementaGrauEntrada(1);
        noOrigem->decrementaGrauSaida(1);
    }
    else
    {
        cout<<" Chmando aux para "<<noOrigem->getId()<<" e "<< noDestino->getId()<<endl;
        auxRemoverArco(noOrigem, idNoDestino);
        
        cout<<" Chmando aux para "<<noDestino->getId()<<" e "<< noOrigem->getId()<<endl;
        auxRemoverArco(noDestino, idNoDestino);
        noOrigem->decrementaGrauEntrada(1);
        noDestino->decrementaGrauEntrada(1);
    }
    this->numArcos -= 1;
}

void Grafo::auxRemoverArco(No *noOrigem, int idNoDestino)
{

    Arco *busca = noOrigem->getArcoAdjacentes();
    if (busca == NULL)
    {
        cout << "Nó " << noOrigem->getId() << " não tem adjacente..." << endl;
        return;
    }

    Arco *predecessor = NULL;

    while (busca != NULL && busca->getNodeDest() != idNoDestino)
    {
        predecessor = busca;
        busca = busca->getProx();
    }

    if (busca->getNodeDest() != idNoDestino)
    {
        cout << "O arco " << idNoDestino << " não existe como adjacente do no " << noOrigem->getId() << endl;
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