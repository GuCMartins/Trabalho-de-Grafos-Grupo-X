#ifndef ARCO_H
#define ARCO_H

class Arco
{
private:
    int nodeDest;
    float w;
    Arco* proxArco;
public:
    Arco(int nodeDest, float w);
    ~Arco();
    float getPeso();
};

#endif // Arco_H