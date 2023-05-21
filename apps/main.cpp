#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/Grafo.h"

using namespace std;

void divideString(string s, float cut[3])
{
    string auxS = "";
    int aux = 0;
    const char *c = s.c_str();
    for (int i = 0; i <= s.length(); i++)
    {
        if (c[i] != ' ' && i != s.length())
        {
            auxS += s[i];
        }
        else
        {
            cut[aux] = stof(auxS);
            aux++;
            auxS = "";
        }
    }
}

Grafo leituraArquivo(string path, string ehDir, string ehPondNode, string ehPondAr)
{
    ifstream arq(path);

    string line;
    string numV;
    string auxId, auxPesoNode;

    Grafo G(0, ehDir == "1", ehPondNode == "1", ehPondAr == "1");

    if (arq.is_open())
    {
        getline(arq, line);
        numV = line;
        if (ehPondNode == "0")
            for (int i = 0; i < stoi(numV); i++)
                G.inserirNo(i, 0);
        else
            for (int i = 0; i < stoi(numV); i++)
            {
                getline(arq, line);
                float cut[3];
                divideString(line, cut);
                int idNode = (int)cut[0];
                float pesoNode = cut[1];
                G.inserirNo(idNode, pesoNode);
            }
        while (getline(arq, line))
        {
            if (ehPondAr == "0")
            {
                float cut[3];
                divideString(line, cut);
                int idNoOrigem = (int)cut[0];
                float idNoDestino = cut[1];
                G.inserirArco(idNoOrigem, idNoDestino, 0);
            }
            else
            {
                float cut[3];
                divideString(line, cut);
                int idNoOrigem = (int)cut[0];
                int idNoDestino = (int)cut[1];
                float pesoArco = cut[2];
                cout << idNoOrigem << " " << idNoDestino << " " << pesoArco << endl;
                G.inserirArco(idNoOrigem, idNoDestino, pesoArco);
            }
        }
    }

    else
        cerr << "Erro ao tentar abrir o arquivo de entrada" << endl;

    return G;
}

int main(int argc, char **argv)
{

    string pathIn = "", pathOut = "";
    string ehDir, ehPondNode, ehPondAr;

    if (argc != 6)
    {
        cout << "Entrada invalida! Tente <nome_do_executavel> <arq_In> <arqOut> <direcionado[0,1]> <ponderadoNode[0,1]> <ponderadoAresta[0,1]>" << endl;
        return 0;
    }

    else
    {
        pathIn = argv[1];
        pathOut = argv[2];
        ehDir = argv[3];
        ehPondNode = argv[4];
        ehPondAr = argv[5];
    }

    // Grafo G = leituraArquivo(pathIn, ehDir, ehPondNode, ehPondAr);
    Grafo *g = new Grafo(5, ehDir == "1", ehPondNode == "1", ehPondAr == "1");
    g->inserirNo(1, 1);
    g->inserirNo(2, 4);
    g->inserirNo(3, 6);
    g->inserirNo(4, 6);

    g->inserirArco(1, 2, 1);
    g->inserirArco(1, 3, 1);

    g->inserirArco(2, 3, 1);
    g->inserirArco(2, 4, 1);

    g->inserirArco(3, 4, 1);

    g->imprimirListaNos();
    g->imprimirListaNosAdjacentes(4);
    // escritaArquivo(pathOut, G) //passar o grafo modificado com base na entrada

    // Continuar o tratamento para o argv

    return 0;
}
