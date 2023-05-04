#ifndef ARCO_H
#define ARCO_H

class Arco
{
private:
    int idDestino;
    float pesoArco;
    Arco* proxArco;
public:
    Arco(int nodeDest, float w);
    ~Arco();
    float getPeso();
};

#endif // Arco_H