#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/Grafo.h"
#include "../include/No.h"
#include "../include/Arco.h"
#include "grafos_util.h"
#include "../include/Cluster.h"

using namespace std;

Cluster** clusters;

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

Grafo* leituraArquivo(string path, string instance, string ehDir, string ehPondAr, string ehPondNode)
{
    ifstream arq(path);

    string line;
    string numV;
    string numCluster;
    string clusterCapmin;
    string clusterCapMax;
    string clusterType;
    string auxId, auxPesoNode;
    
    if (arq.is_open())
    {

        if(instance == "Handover"){
            getline(arq, line);
            numV = line;
            getline(arq, line);
            numCluster = line;
            getline(arq, line);
            clusterCapMax = line;
            clusterType = "ss";

            clusters = new Cluster*[stoi(numCluster)];


            Grafo *G = new Grafo(stoi(numV), false, true, true);
            
            for(int i = 0 ; i < stoi(numCluster); i++){
            
                clusters[i] = new Cluster(0, stoi(clusterCapMax), G, clusterType);
            }
            
            for(int i = 0; i < stoi(numV); i++){
                getline(arq, line);
                string pesoNode = line;
                G->inserirNo(i, stof(pesoNode));
            }

            getline(arq, line);
            const char* matrix = line.c_str();
            int n = stoi(numV);
            int **M = new int *[n];

            for (int i = 0; i < n; i++)
                M[i] = new int[n];        

            int i = 0, j = 0, percorre = 0;
            char c;
            string s = "";
            char m = matrix[percorre];

            while(j < n){
                m = matrix[percorre];
                percorre++;
                if(m != ' ')
                    s += m;
                else{
                    M[j][i] = stof(s);
                    // if(s != "0"){
                    //     cout << "inserindo: " << s << " em: " << j << "," << i << endl;
                    // }
                    s = "";
                }
                if(i < n && s == "")
                    i++;
                else if(i == n){
                    i = 0;
                    j++;
                }
            }


           for(int k = 0; k < n; k++)
            for(int l = 0; l < n; l++){
                if(k != l)
                    G->inserirArco(k, l, M[k][l]);
            }
            return G;
        }

        if(instance == "RanReal240" || instance == "RanReal480" || instance == "RanReal960" || instance == "Sparse82")
        {   
            getline(arq, line);
            const char* linha = line.c_str();
            int aux = 0;
            char c = linha[aux];
            
            string s = "";
            bool nC = false; //indica se pegou o numero de clusters
            bool cT = false; //indica se pegou o tipo do cluseter
            bool cLmin = false; //indica se pegou o limite inferior do cluster
            bool cLmax = false; //indica se pegou o limite superior do cluster
            bool w = false; //indica que alcançou a letra W para começar a ler os pesos dos nós
            int id = 0; //ids dos nos
            int contadorCluster = 0; //auxiliar para indicar o cluster que ele está atribuindo os parâmetros

            while(c != ' ')
            {
                c = linha[aux];
                aux++;
                s += c;
            }
            
            int n = stoi(s);

            Grafo *G = new Grafo(n, false, true, true);

            c = linha[aux + 1];
            s = "";

            while(c != '\0'){
                c = linha[aux];
                aux++;

                if(!nC){
                    if(c != ' ')
                        s += c;
                    else{
                        numCluster = s;
                        nC = true;
                        s = "";
                        clusters = new Cluster*[stoi(numCluster)];
                    }
                }

                else if(nC && !cT){
                    if(c != ' ')
                        s += c;
                    else{
                        clusterType = s;
                        cT = true;
                        s = ""; 
                    }
                }

                else if(cT && !cLmin){
                    if(c != ' ')
                        s += c;
                    
                    else{
                        clusterCapmin = s;
                        cLmin = true;
                        cLmax = false;
                        s = "";
                    }
                }

                else if(cLmin && !cLmax && c != 'W'){
                    if(c != ' ')
                        s += c;
                    else{
                        clusterCapMax = s;
                        cLmax = true;
                        cLmin = false;
                        s = "";
                        clusters[contadorCluster] = new Cluster(stoi(clusterCapmin), stoi(clusterCapMax), G, clusterType);
                        contadorCluster++;
                    }
                }

                if(c == 'W'){
                    cLmax = true;
                    cLmin = true;
                    w = true;
                    s = "";
                    aux++;
                }
                if(w == true && c != 'W'){
                    if(isdigit(c))
                        s += c;
                    else{
                        if(c == ' ' && s != ""){
                            G->inserirNo(id, stof(s));
                            s = "";
                            id++;
                        }
                    }
                }
            }

            if(s != ""){
                G->inserirNo(id, stof(s));
            }

            while (getline(arq, line))
            {
                    float cut[3];
                    divideString(line, cut);
                    int idNoOrigem = (int)cut[0];
                    int idNoDestino = (int)cut[1];
                    float pesoArco = cut[2];
                    G->inserirArco(idNoOrigem, idNoDestino, pesoArco);
            }

            return G;
        }
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
    string ehDir, ehPondNode, ehPondAr, instance;

    if (argc != 7)
    {
        cout << "Entrada invalida! Tente <nome_do_executavel> <arq_In> <arqOut> <direcionado[0,1]> <ponderadoNode[0,1]> <ponderadoAresta[0,1]>" << endl;
        return 0;
    }

    else
    {
        pathIn = argv[1];
        pathOut = argv[2];
        instance = argv[3];
        ehDir = argv[4];
        ehPondAr = argv[5];
        ehPondNode = argv[6];
    }

    Grafo *G = leituraArquivo(pathIn, instance, "0", "1", "1");

    // float** matriz = floydWarshalAlgorithm(G);
    // cout << "---------------------------------------------" << endl;
    // for(int i = 0; i < G->getOrdem(); i++){
    //     for(int j = 0; j < G->getOrdem(); j++){
    //         cout << matriz[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // float* distancias = dijkstraAlgorithm(G, 4);

    // for(int i = 0; i < G->getOrdem(); i++){
    //     cout << "Id do no: " << i+1 << " | Caminho minimo ate ele: " << distancias[i] << endl;
    // }

    // Grafo* res = kruskalAlgorithm(G);
    
    
    //escritaArquivo(pathOut, G);
    
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