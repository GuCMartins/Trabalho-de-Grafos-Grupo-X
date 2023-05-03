#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include "Arco.h"

class Grafo
{
private:
    int ordem,nArcos;
    No *NoInicial;
    bool digrafo,pondAresta,pondNode;
public:
    Grafo(int ordem, bool digrafo, bool pondAresta, bool pondNode);
    ~Grafo();
    void inserirNo(int id, float peso);
    void removerNo(int id);
    void inserirArco(int id, int idNoOrigem, int idNoDestino, float peso);
    void removerArco(int id, int idNoOrigem, int idNoDestino);
    int *getGrauNo(int id);
    int getOrdem();
    int getRegularidade();
    int *vizinhancaAberta(int idNo);   
    int *vizinhancaFechada(int idNo);    
};
#endif // GRAFO_H