#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/Grafo.h"
#include "../include/No.h"
#include "../include/Arco.h"
#include "grafos_util.h"

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

Grafo* leituraArquivo(string path, string ehDir, string ehPondAr, string ehPondNode)
{
    ifstream arq(path);

    string line;
    string numV;
    string auxId, auxPesoNode;

    

    if (arq.is_open())
    {
        getline(arq, line);
        numV = line;
        Grafo *G = new Grafo(stoi(numV), ehDir == "1", ehPondAr == "1", ehPondNode == "1");
        if (ehPondNode == "0")
            for (int i = 0; i < stoi(numV); i++)
                G->inserirNo(i+1, 0);
        else
            for (int i = 0; i < stoi(numV); i++)
            {
                getline(arq, line);
                float cut[3];
                divideString(line, cut);
                int idNode = (int)cut[0];
                float pesoNode = cut[1];
                G->inserirNo(idNode, pesoNode);
            }
        while (getline(arq, line))
        {
            if (ehPondAr == "0")
            {
                float cut[3];
                divideString(line, cut);
                int idNoOrigem = (int)cut[0];
                float idNoDestino = cut[1];
                G->inserirArco(idNoOrigem, idNoDestino, 0);
            }
            else
            {
                float cut[3];
                divideString(line, cut);
                int idNoOrigem = (int)cut[0];
                int idNoDestino = (int)cut[1];
                float pesoArco = cut[2];
                cout << idNoOrigem << " " << idNoDestino << " " << pesoArco << endl;
                G->inserirArco(idNoOrigem, idNoDestino, pesoArco);
            }
        }
        return G;
    }

    else
        cerr << "Erro ao tentar abrir o arquivo de entrada" << endl;

    arq.close();
    return NULL;
}

void escritaArquivo(string pathOut, Grafo *G){

    ofstream arqSaida(pathOut);
    
    if(arqSaida.is_open()){
        arqSaida << to_string(G->getOrdem()) + "\n";

        if(G->ehDir()){
            if(G->ehPondNode()){
                No *noInicial = G->getNoInicial();
                while(noInicial != NULL){
                    arqSaida << to_string(noInicial->getId()) + " " + to_string(noInicial->getGrauEntrada()) + to_string(noInicial->getGrauSaida()) + "\n"; 
                    noInicial = noInicial->getProx();
                }
            }

            if(G->ehPondAr())
            {
                No *no = G->getNoInicial();
                while(no != NULL){
                    Arco *arco = no->getAdjacentes();
                    while(arco != NULL){
                        arqSaida << to_string(no->getId()) + " " + to_string(arco->getNodeDest()) + " " + to_string(arco->getPeso()) + "\n"; 
                        arco = arco->getProx();
                    }
                    no = no->getProx();
                }

            }

            else{
                No *no = G->getNoInicial();
                while(no != NULL){
                    Arco *arco = no->getAdjacentes();
                    while(arco != NULL){
                        arqSaida << to_string(no->getId()) + " " + to_string(arco->getNodeDest()) + "\n"; 
                        arco = arco->getProx();
                    }
                    no = no->getProx();
                }
            }
        }
        else{
            if(G->ehPondNode()){
                No *noInicial = G->getNoInicial();
                while(noInicial != NULL){
                    arqSaida << to_string(noInicial->getId()) + " " + to_string(noInicial->getGrauEntrada()) + "\n";
                    noInicial = noInicial->getProx(); 
                }
            }

            if(G->ehPondAr())
            {
                No *no = G->getNoInicial();
                int *visitados = new int[G->getOrdem()];
                int v = 1;
                bool visitado = false;
                while(no != NULL){
                    Arco *arco = no->getAdjacentes();
                    visitados[v-1] = no->getId();
                    while(arco != NULL){
                        for(int i = 0; i < v; i++){
                            if(arco->getNodeDest() == visitados[i])
                                visitado = true;
                        }
                        if(visitado)
                            arqSaida << to_string(no->getId()) + " " + to_string(arco->getNodeDest()) + " " + to_string(arco->getPeso()) + "\n";
                        arco = arco->getProx();
                        visitado = false;
                    }
                    no = no->getProx();
                    v++; 
                    delete arco;
                }

                delete [] visitados;
            }

            else{
                No *no = G->getNoInicial();
                int *visitados = new int[G->getOrdem()];
                int v = 1;
                bool visitado = false;
                while(no != NULL){
                    Arco *arco = no->getAdjacentes();
                    visitados[v-1] = no->getId();
                    while(arco != NULL){
                        for(int i = 0; i < v; i++){
                            if(arco->getNodeDest() == visitados[i])
                                visitado = true;
                        }
                        if(visitado)
                            arqSaida << to_string(no->getId()) + " " + to_string(arco->getNodeDest()) + "\n";
                        arco = arco->getProx();
                        visitado = false;
                    }
                    no = no->getProx();
                    v++; 
                    delete arco;
                }

                delete [] visitados;
            }
        }
    }
    else
        cerr << "Erro ao tentar abrir o arquivo .txt" << endl;
    arqSaida.close();
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
        ehPondAr = argv[4];
        ehPondNode = argv[5];
    }

    Grafo *G = leituraArquivo(pathIn, ehDir, ehPondAr, ehPondNode);

    float** matriz = floydWarshalAlgorithm(G);
    cout << "---------------------------------------------" << endl;
    for(int i = 0; i < G->getOrdem(); i++){
        for(int j = 0; j < G->getOrdem(); j++){
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
    // float* distancias = dijkstraAlgorithm(G, 4);

    // for(int i = 0; i < G->getOrdem(); i++){
    //     cout << "Id do no: " << i+1 << " | Caminho minimo ate ele: " << distancias[i] << endl;
    // }

    // Grafo* res = kruskalAlgorithm(G);
    
    // escritaArquivo(pathOut, res);
    
    // cout << "Impressão da lista de nos: " << endl;
    // G->imprimirListaNos();

    // cout << "Resultado do isCycle: " << isCycle(G) << endl;
    
    //cout <<"IMPRESSÃO "<<endl;
    //G->imprimirTodosNosAdjacentes();

    // cout <<"REMOVER NO "<<endl;
    // G->removerNo(5);
    
    // cout <<"IMPRESSÃO APÓS REMOVER "<<endl;
    //  G->imprimirTodosNosAdjacentes();

    

    //escritaArquivo(pathOut, G); //passar o grafo modificado com base na entrada

    //cout << "Eh no articulacao? " << endl << ehNoArticulacao(G, 5) << endl;

    //Grafo* copia = criarCopia(G);


    //copia->imprimirTodosNosAdjacentes();

    // int k = 4;
    // cout << "Verificando se o grafo é "<<k<<"-regular"<<endl;
    // ehKRegular(G, k);

    
    // int id = 1;
    // No *no = getGrauNo(G, id);
    // if(no!=NULL){
    //     cout <<"GE: "<<no->getGrauEntrada()<<endl;
    //     if(G->ehDir())
    //         cout <<"GS: "<<no->getGrauSaida()<<endl;
    // }else{
    //     cout <<"Nó com id "<<id<< " inválido..."<<endl;
    // }
    

    // int size = 4;
    // int *idNos = new int[size];
    
    // idNos[0] = 1;
    // idNos[1] = 2;
    // idNos[2] = 3;
    // idNos[3] = 4;
   
    // Grafo *grafoInduzido = subgrafoInduzido(G, idNos, &size);
    // cout <<"Ordem do subgrafo: "<<grafoInduzido->getOrdem()<<endl;
    // cout <<"Num de arcos: "<<grafoInduzido->getNumArcos()<<endl;
    // grafoInduzido->imprimirTodosNosAdjacentes();
    
    
    //G->imprimirTodosNosAdjacentes();
    //cout <<"Imprimindo subgrafo induzido"<<endl;
    //grafoInduzido->imprimirTodosNosAdjacentes();

    // Grafo *G2 = new Grafo(3, ehDir == "1", ehPondAr == "1", ehPondNode == "1");
    // G2->inserirNo(1,1);
    // G2->inserirNo(2,1);
    // G2->inserirNo(3,1);

    // G2->inserirArco(1,2,3);
    // G2->inserirArco(2,2,3);
    // G2->imprimirTodosNosAdjacentes();


    // Grafo *G3 = new Grafo(3, ehDir == "1", ehPondAr == "1", ehPondNode == "1");
    // G3->inserirNo(1,1);
    // G3->inserirNo(2,1);
    // G3->inserirNo(3,1);

    // //G3->inserirArco(1,2,1);
    // G3->inserirArco(1,3,1);

    // //G3->inserirArco(2,1,1);
    // G3->inserirArco(2,3,1);

    // G3->inserirArco(3,1,1);
    // G3->inserirArco(3,2,1);

    // Grafo *grafoC = grafoComplementar(G3);
    //  cout <<"Ordem do subgrafo: "<<grafoC->getOrdem()<<endl;
    // cout <<"Num de arcos: "<<grafoC->getNumArcos()<<endl;
    // grafoC->imprimirTodosNosAdjacentes();


    // delete idNos;

    // Continuar o tratamento para o argv
    return 0;
}