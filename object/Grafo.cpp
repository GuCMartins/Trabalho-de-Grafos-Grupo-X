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
    this->matrizAdj = new int*[ordem];
    for(int i =0;i<ordem;i++){
        matrizAdj[i] = new int[ordem];
        for(int j=0;j<ordem;j++){//preenche cada linha da matriz com 0
            matrizAdj[i][j] = 0;
        }
    }
}
Grafo::~Grafo() {}

void Grafo::inserirNo(int idNode, float pesoNode)
{
    if(findNoById(idNode) != NULL){
        cout <<"Nó "<<idNode<<" já existe no grafo."<<endl;
        return;
    }

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
    if(idNoOrigem == idNoDestino){
        cout <<"Grafo simples. Não é permitido self-loop."<<endl;
        return;
    }

    No *noOrigem = this->findNoById(idNoOrigem);
    
    if(noOrigem == NULL){
        cout <<"Não é possível inserir o arco <"<<idNoOrigem<<","<<idNoDestino<<">. Nó Origem"<<idNoOrigem<< " não existe no grafo"<<endl;
        return;
    }

    if( noOrigem->existeNoAdjacente(idNoDestino) != NULL){
        // cout <<"Já existe o adjacente "<<idNoDestino<< " para o nó "<<idNoOrigem<<endl;
        return;
    }

    No *noDestino = this->findNoById(idNoDestino);

    if(noDestino == NULL){
        cout <<"Não é possível inserir o arco <"<<idNoOrigem<<","<<idNoDestino<<">. Nó Destino"<<noDestino<< " não existe no grafo"<<endl;
        return;
    }

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
    if(!ehDir())
        matrizAdj[idNoDestino-1][idNoOrigem-1] = 1;
    matrizAdj[idNoOrigem-1][idNoDestino-1] = 1;
    
}

void Grafo::auxInserirArco(No *noOrigem, No *noDestino, float pesoArco)
{
    Arco *novoArco = new Arco(noDestino->getId(), pesoArco);

    Arco *arcoInsercao = noOrigem->getAdjacentes();

    if (arcoInsercao == NULL)
    {
        noOrigem->setAdjacente(novoArco);
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

    Arco *adjacentes = noRemover->getAdjacentes();

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
        this->ordem -= 1;
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

    noRemover->setAdjacente(NULL);

    if(noRemover->getId() == noInicial->getId()){
            noInicial = noRemover->getProx();
    }else{
        predecessor->setProx(noRemover->getProx());
    }
    //modifica a matriz de adjacencia para retirar o no
    for(int i=0;i<ordem;i++){
        matrizAdj[i][idNode-1] = 0;
        matrizAdj[idNode-1][i] = 0;
    }
    cout <<" Removendo nó "<<noRemover->getId()<<endl;
    delete noRemover;
    this->ordem -= 1;
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
    matrizAdj[idNoOrigem-1][idNoDestino-1] = 0;//modifica a matriz de adjacencia para retirar o arco
}

void Grafo::auxRemoverArco(No *noOrigem, int idNoDestino)
{

    Arco *busca = noOrigem->getAdjacentes();
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
        noOrigem->setAdjacente(busca->getProx());
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
    Arco *arco = no->getAdjacentes();
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

bool Grafo::existeArco(int noPartida,int noDestino){//faz a busca pela matriz de adjacencia para ver se existe um arco entre os nos
    if(direcionado)
        if(matrizAdj[noPartida-1][noDestino-1] == 1)
            return true;
    else
        if(matrizAdj[noPartida-1][noDestino-1] == 1 || matrizAdj[noDestino-1][noPartida-1] == 1)
            return true;
    return false;                
}