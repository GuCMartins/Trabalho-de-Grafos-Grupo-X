#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include "Arco.h"

class Grafo
{
private:
    int ordem;
    int regularidade;
    No** Nos;
    Arco** Arcos;
    int numArcos;
public:
    Grafo(/* args */);
    ~Grafo();
    void inserirNo(int id, int x, int y, float peso);
    void removerNo(int id);
    void inserirArco(int id, int idNoOrigem, int idNoDestino, float peso);
    void removerArco(int id);
    int* getGrauNo(int id);
    int getOrdem(){return ordem;}
    int getRegularidade();
    int getNumArcos(){return numArcos;}
    Arco* getArcos(){return Arcos;}  
};
#endif // GRAFO_H