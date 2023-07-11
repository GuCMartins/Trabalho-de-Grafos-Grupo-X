#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/Grafo.h"
#include "../include/No.h"
#include "../include/Arco.h"
#include "grafos_util.h"
#include "../include/Cluster.h"
#include "greedy_algorithms.h"
// #include <unistd.h>


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

Grafo* leituraArquivoFase1(string path, string ehDir, string ehPondAr, string ehPondNode)
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


Grafo* leituraArquivoFase2(string path, string instance, string ehDir, string ehPondAr, string ehPondNode)
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
                
                clusters[i] = new Cluster(0, stof(clusterCapMax), G, clusterType);
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
                if(M[k][l] != 0){
                    if(k != l)
                        G->inserirArco(k, l, M[k][l]);
                }
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
                        clusters[contadorCluster] = new Cluster(stoi(clusterCapmin), stof(clusterCapMax), G, clusterType);
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

void clearScreen() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

int main(int argc, char **argv)
{

        string pathIn = "", pathOut = "";
        string ehDir, ehPondNode, ehPondAr, instance;

        if (argc != 6)
        {
            cout << "Entrada invalida! Tente <nome_do_executavel> <arq_In> <arqOut> <direcionado[0,1]> <ponderadoNode[0,1]> <ponderadoAresta[0,1]>" << endl;

            // cout << argv[0] << endl;
            // cout << argv[1] << endl;
            // cout << argv[2] << endl;
            // cout << argv[3] << endl;
            // cout << argv[4] << endl;
            // cout << argv[5] << endl;
            // cout << argv[6] << endl;
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

        // Usar o que está abaixo para ler e escreve o grafo dos algoritmos da primeira etapa.
        Grafo *G = leituraArquivoFase1(pathIn, ehDir, ehPondAr, ehPondNode);
        escritaArquivo(pathOut, G);
        // G->imprimirTodosNosAdjacentes();

        char opcao = ' ';

        while (opcao != 'n')
        {
            cout << "==================================== Menu ====================================" << endl;
            cout <<"1. Limpar tela\n";
            cout <<"2. Sair\n";
            cout << "h) mostrar a vizinhança aberta de um dado nó informado pelo usuário\n";
            cout << "i) mostrar a vizinhança fechada de um dado nó informado pelo usuário\n";
            cout << "l) verificar se o grafo é bipartido\n";
            cout << "o) retornar o fecho transitivo direto de um dado nó informado pelo usuário\n";
            cout << "p) retornar o fecho transitivo indireto de um dado nó informado pelo usuário\n";
            cout << "r) apresentar o subgrafo induzido por um dado conjunto de vértices informado pelo usuário\n";
            cout << "t) para digrafos, apresentar as componentes fortemente conexas\n";
            cout << "u) verificar se o grafo é eulerianos\n";
            cout << "v) apresentar os nós de articulação";
            cout << "w) apresentar as arestas ponte\n";
            cout << "y) apresentar o raio, o diâmetro, o centro e a periferia dografo\n";
            cout << "z) Apresentar o caminho mínimo entre dois vértices usando o algoritmo de Dijkstra ou de Floyd (escolha do usuário).\n";

            // cout << "Digite qual a letra, dentre as avaliativas, da função que você deseja utilizar. Digite a letra 'n' a qualquer momento para pausar o display..." << endl;
            cin >> opcao;
            int idNo;
            int noOrigem, noDestino;
            switch (opcao){
                case '1':
                {
                    clearScreen();
                    break;
                }
                case '2':
                {
                    exit(0);
                    break;
                }
                case 'h':
                {
                    cout << "Digite o id do no que será avaliada a vizinhança aberta: " << endl;
                    cin >> idNo;
                    cout << "ID no: " << idNo << endl;

                    G->vizinhancaAberta(idNo);
                    break;
                }
                case 'i':
                {
                    cout << "Digite o id do no que será avaliada a vizinhança fechada: " << endl;
                    cin >> idNo;
                    G->vizinhancaFechada(idNo);
                    break;
                }
                case 'l':
                {
                    clearScreen();
                    if(isBipartite(G)){
                        cout << "O grafo lido é bipartido!"<< endl;
                    }else{
                        cout << "O grafo lido não é bipartido!"<< endl;
                    }

                    break;
                }
                case 'o':
                {
                    cout << "Digite o id do no que será avaliada o fecho transitivo direto: " << endl;
                    cin >> idNo;
                    G->FechoTransitivoDireto(idNo);
                    break;
                }
                case 'p':
                {
                    cout << "Digite o id do no que será avaliada a fecho transitivo indireto: " << endl;
                    cin >> idNo;
                    G->FechoTransitivoIndireto(idNo);
                    break;
                }
                case 'r':
                {
                    int numeroVertices;
                    cout << "Informe o número de vértices: " << endl;
                    cin >> numeroVertices;
                    int *idNosVet = new int[numeroVertices];
                    cout << "Informe os ids dos vértices: " << endl;
                    int idV;
                    for (int k = 0; k < numeroVertices; k++)
                    {
                        cin >> idNosVet[k];
                    }
                    Grafo *induzido = subgrafoInduzido(G, idNosVet, numeroVertices);
                    cout << "Grafo induzido pelos vértices informados: " << endl;
                    induzido->imprimirTodosNosAdjacentes();
                    break;
                }
                case 't':
                {
                    componentesFortementeConexas(G);
                    break;
                }
                case 'u':
                {
                    cout << "É euleriano ? " << G->Euleriano() << endl;
                    break;
                }
                case 'v':
                {
                    cout << "Digite o id do no que sera verificado a propriedade: " << endl;
                    cin >> idNo;
                    cout << "É nó de articulação ? " << ehNoArticulacao(G, idNo) << endl;
                    break;
                }
                case 'w':
                {
                    cout << "Digite o id do nó de origem: " << endl;
                    cin >> noOrigem;
                    cout << "Digite o id do nó de destino: " << endl;
                    cin >> noDestino;
                    cout << "É aresta ponte ? " << ehArestaPonte(G, noOrigem, noDestino) << endl;
                    break;
                }
                case 'y':
                {
                    Grafo *g = kruskalAlgorithm(G);
                    g->imprimirTodosNosAdjacentes();
                    break;
                }
                case 'z':
                {
                    char opt;
                    cout << "Qual o nó de origem ?" << endl;
                    cin >> noOrigem;
                    cout << "Qual o nó de destino ?" << endl;
                    cin >> noDestino;
                    cout << "Deseja utilizar o algoritmo de Dijkstra(d) ou de Floyd(f)?" << endl;
                    cin >> opt;
                    if (opt == 'd')
                    {
                        float *valores = dijkstraAlgorithm(G, noOrigem);
                        cout << "Valor do caminho mínimo: " << valores[noDestino] << endl;
                    }
                    else if (opt == 'f')
                    {
                        float **valores = floydWarshalAlgorithm(G);
                        cout << "Valor do caminho mínimo: " << valores[noOrigem][noDestino] << endl;
                    }
                    break;
                }
                case 'n':
                {
                    cout << "Saindo do display..." << endl;
                    break;
                }
                
            }
            
        }

        return 0;
}