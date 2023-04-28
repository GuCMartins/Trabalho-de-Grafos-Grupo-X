#ifndef GRAFOSUTIL_CPP
#define GRAFOSUTIL_CPP

#include <iostream>

using namespace std;

/*
    => Percorro todas as arestas verificando se o id entre dois caras em sequência
    é igual. Caso seja, é multigrafo. Caso contrário, retorno falso.
    => Mudar a lógica na verificação do if, pois ainda não está correto.
    => Problema: essa verificação só vale se a lista encadeada estiver ordenada.
    Conversar com a pessoa que fará função de inserção para tentar inserir orde-
    nado.
*/
bool ehMultigrafo(Grafo g){
    for(No* v = g.Nos; p != NULL; v = v->getProxNo()){
        for(Arco *a = g.Arcos; a != NULL; a-->getProxArco()){
            if(a.getId() == a.getProxArco().getId())
                return true;
        }
    }
    return false;
}

/*
    => Verifico se a quantidade de arestas no grafo corresponde a n*(n-1)/2,
    visto que se um grafo é completo. Além disso, é necessário verificar se
    o grafo é multigrafo ou não, pois a primeira verificação fura se desconsi-
    derar multigrafos.
    => Exemplo: grafo k3, com arestas A, B e C. Se pegarmos um multigrafo A,B,C
    o mesmo não é completo mas satisfaz a primeira propriedade. Grafos com self-
    loops também caem no mesmo problema. Botar condição para isso depois.
*/
bool ehCompleto(Grafo g){
    if(g.getNumArcos() == g.getOrdem()*(g.getOrdem()-1)/2 && !ehMultigrafo(g))
        return true;
    else
        return false;
}

/*
    => Grau do grafo é o maior valor para de grau dentre os vértices do grafo.
    => Seria verificar em cada nó a quantidade de itens na lista de arcos dele. Caso
    alguém faça a parte de sequência de grau, seria interessante mudar essa função.
*/
int grauGrafo(Grafo g){
    int max = -1, cont;
    for(No* v = g.Nos; p != NULL; v = v->getProxNo()){
        cont = 0;
        for(Arco *a = g.Arcos; a != NULL; a-->getProxArco()){
            cont++;
        }
        if(cont > max || max == -1)
            max = cont;
    }
    return max;
}

bool ehNulo(Grafo g){
    if(g.getArcos() == NULL)
        return true;
    else
        return false;
}

bool ehTrivial(Grafo g){
    if(g.getArcos() == NULL && g.getOrdem() == 1)
        return true;
    else
        return false;
}

#endif
