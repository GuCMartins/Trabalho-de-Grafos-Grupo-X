#ifndef ARCO_H
#define ARCO_H

class Arco
{
private:
    int idDestino;
    int pesoArco;
    Arco *proxArco;
public:
    Arco(/* args */);
    ~Arco();
    int getPeso();
};

#endif // Arco_H