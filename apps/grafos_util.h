#ifndef GRAFOSUTIL_CPP
#define GRAFOSUTIL_CPP

#include <iostream>
#include "../include/Grafo.h"

using namespace std;    


/*
    => Verifico se a quantidade de arestas no grafo corresponde a n*(n-1)/2,
    visto que se um grafo é completo. Além disso, é necessário verificar se
    o grafo é multigrafo ou não, pois a primeira verificação fura se desconsi-
    derar multigrafos.
    => Exemplo: grafo k3, com arestas A, B e C. Se pegarmos um multigrafo A,B,C
    o mesmo não é completo mas satisfaz a primeira propriedade. Grafos com self-
    loops também caem no mesmo problema. Botar condição para isso depois.
*/
bool ehCompleto(Grafo *g){
    if(g->getNumArcos() == g->getOrdem()*(g->getOrdem()-1)/2)
        return true;
    else
        return false;
}

/*
    => Grau do grafo é o maior valor para de grau dentre os vértices do grafo.
    => Seria verificar em cada nó a quantidade de itens na lista de arcos dele. Caso
    alguém faça a parte de sequência de grau, seria interessante mudar essa função.
*/
int grauGrafo(Grafo* g){
    //testar se o grafo é direcionado;
    if(!g->ehDirecionado()){
        int max = -1;
        for (No *v = g->getNoInicial(); v != NULL; v = v->getProx())
        {
            if (v->getGrauEntrada() > max || max == -1)
                max = v->getGrauEntrada();
        }
        return max;
    }else{
        cout << "Grafo eh direcionado." << endl;
        return -1;
    }
}

bool ehNulo(Grafo *g){
    for (No* node = g->getNoInicial(); node != NULL; node = node->getProx()) 
        if(node->getGrauEntrada() != 0)
            return false;
    return true;
}

bool ehTrivial(Grafo *g){
    if(g->getOrdem() == 1)
        return true;
    else
        return false;
}

#endif
