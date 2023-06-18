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
Grafo::~Grafo() {
    //Percorrer e ficar dando delete nesse treco
}

void Grafo::inserirNo(int idNode, float pesoNode)
{
    if(findNoById(idNode) != NULL){
        cout <<"Nó "<<idNode<<" já existe no grafo."<<endl;
        return;
    }

    No *novoNo = new No(idNode, pesoNode);

    No *antigoNoInicial = noInicial;
    this->noInicial = novoNo;
    novoNo->setProx(antigoNoInicial);
    
    this->ordem += 1;
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
    
}

void Grafo::auxInserirArco(No *noOrigem, No *noDestino, float pesoArco)
{
    Arco *novoArco = new Arco(noDestino->getId(), pesoArco);

    Arco *arcoInicialAtual = noOrigem->getAdjacentes();
    noOrigem->setAdjacente(novoArco);
    novoArco->setProx(arcoInicialAtual);
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

    this->ordem -= 1;

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
        auxRemoverArco(noDestino, idNoOrigem);
        noOrigem->decrementaGrauEntrada(1);
        noDestino->decrementaGrauEntrada(1);
    }
    this->numArcos -= 1;
    //matrizAdj[idNoOrigem-1][idNoDestino-1] = 0;//modifica a matriz de adjacencia para retirar o arco
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

int *Grafo::vizinhancaAberta(int idNo){
    int size = 0;
    No* no = findNoById(idNo);
    if(direcionado)
        size = no->getGrauSaida() + no->getGrauEntrada();
    else
        size = no->getGrauEntrada();    
    int *vizinhanca = new int[size];//aloca um vetor de vizinhanca com o numero de vizinhos do no
    int i = 0;
    Arco* arco = this->findNoById(idNo)->getAdjacente();//pega a aresta que sai do nó
    while(i<size && arco != NULL){
        if(arco->getNodeDest() == idNo || arco->getProx()->getNodeDest() == idNo){
            vizinhanca[i] = arco->getProx()->getNodeDest();
            i++;
        }
    }
    return vizinhanca;
}

int *Grafo::vizinhancaFechada(int idNo)
{
    int size = 0;
    No* no = findNoById(idNo);
    if(direcionado)
        size = no->getGrauSaida() + no->getGrauEntrada();
    else
        size = no->getGrauEntrada();    
    int *vizinhancaAux = new int[size];//aloca um vetor de vizinhanca com o numero de vizinhos do no
    int *vizinhanca = new int[size+1];
    vizinhancaAux = vizinhancaAberta(idNo);//recebe a vizinhanca aberta e completa com o no que esta sendo analisado
    for(int i =0;i<size;i++)
        vizinhanca[i] = vizinhancaAux[i];
    vizinhanca[size] = idNo;
    return vizinhanca;
}

bool Grafo::Euleriano(int *visitados){//recebe um vetor de visitados do DFS
//Teorema: Um multigrafo M é euleriano se e somente se M é conexo e cada vértice de M tem grau par.
    
    if(ordem == 0)
        return false;//se o grafo nao tiver nos, nao eh euleriano
    if(ordem == 1)
        return true;//se o grafo tiver apenas um no, é euleriano

    for(int i=0;i<ordem;i++){
        if(visitados[i] == 0)
            return false;//se algum no nao foi visitado, o grafo nao eh euleriano
        if(findNoById(i)->getGrauEntrada()%2!=0 || findNoById(i)->getGrauSaida()%2 !=0)
            return false;//se algum no tiver grau de entrada diferente do grau de saida, o grafo nao eh euleriano   
    }
    return true;
}

bool Grafo::existeArco(int noPartida,int noDestino){//faz a busca pela matriz de adjacencia para ver se existe um arco entre os nos
    No *no = findNoById(noPartida);
    Arco *arco = no->getAdjacentes();
    while(arco != NULL){
        if(arco->getNodeDest() == noDestino)
            return true;
        arco = arco->getProx();    
    }
    return false;                
}

bool Grafo::DFSCaminho(No *noPartida,No* Destino, Grafo *g,int* visitado){//o no de partida é iniciado como o no inicial do grafo
    visitado[noPartida->getId() - 1] = 1;
    Arco *arco = noPartida->getAdjacentes();

    while(arco != NULL){
        if(arco->getNodeDest() == Destino->getId())
            return true;
        if(visitado[arco->getNodeDest() - 1] == -1)
            DFSGeral(g->findNoById(arco->getNodeDest()), g, visitado);
        arco = arco->getProx();
    }
    return false;//usar para fecho transitivo direto e indireto
}

void Grafo::DFSGeral(No *noPartida, Grafo *g, int* visitado){//o no de partida é iniciado como o no inicial do grafo
    visitado[noPartida->getId()] = 1;
    Arco *arco = noPartida->getAdjacentes();

    while(arco != NULL){
        if(visitado[arco->getNodeDest() - 1] == -1)
            DFSGeral(g->findNoById(arco->getNodeDest()), g, visitado);
        arco = arco->getProx();
    }
}

int* Grafo::FechoTransitivoDireto(int idNo){ //o conjunto dos vértices de um grafo alcançáveis a partir de v.
    
    No* noPartida = findNoById(idNo);
    int *fecho = new int[ordem];
    int *visitados = new int[ordem];
    for(int k=0;k<ordem;k++)
        visitados[k] = 0;
    int i = 0;
    for(int j=0;j<ordem;j++){//faz o percurso de busca em profundidade para cada no do grafo, variando o no final
        if(DFSCaminho(noPartida,findNoById(j),this,visitados)){//se encontrar o no a partir do no inicial, adiciona no fecho transitivo
            fecho[i] = j;
            i++;
        }
        for(int k=0;k<ordem;k++)
        visitados[k] = 0;
    }
    return fecho;
}

int* Grafo::FechoTransitivoIndireto(int idNo){ //o conjunto dos vértices de um grafo alcançáveis a partir de v.
    
    No* noDestino = findNoById(idNo);
    int *fecho = new int[ordem];
    int *visitados = new int[ordem];
    for(int k=0;k<ordem;k++)
        visitados[k] = 0;
    int i = 0;
    for(int j=0;j<ordem;j++){//faz o percurso de busca em profundidade para cada no do grafo, variando o no inicial
        if(DFSCaminho(findNoById(j),noDestino,this,visitados)){//se encontrar o no a partir do no inicial, adiciona no fecho transitivo
            fecho[i] = j;
            i++;
        }
        for(int k=0;k<ordem;k++)
        visitados[k] = 0;
    }
    return fecho;
}
