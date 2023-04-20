#include <iostream>
#include "../include/Grafo.h"

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

int main(int, char**) {
    std::cout << "Hello, world!\n";
}
