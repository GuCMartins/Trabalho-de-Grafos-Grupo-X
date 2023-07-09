#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/Grafo.h"
#include "../include/No.h"
#include "../include/Arco.h"
#include "grafos_util.h"
#include "../include/Cluster.h"
#include "greedy_algorithms.h"
#include <chrono>

using namespace std;

Cluster **clusters;

string getFileName(const string &filePath)
{
    size_t lastSlash = filePath.find_last_of("/\\");
    if (lastSlash != std::string::npos)
    {
        return filePath.substr(lastSlash + 1);
    }
    return filePath;
}
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

Grafo *leituraArquivoFase1(string path, string ehDir, string ehPondAr, string ehPondNode)
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
                G->inserirNo(i + 1, 0);
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

Grafo *leituraArquivoFase2(string path, string instance, string ehDir, string ehPondAr, string ehPondNode)
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

        if (instance == "Handover")
        {
            getline(arq, line);
            numV = line;
            getline(arq, line);
            numCluster = line;
            getline(arq, line);
            clusterCapMax = line;
            clusterType = "ss";

            clusters = new Cluster *[stoi(numCluster)];

            Grafo *G = new Grafo(stoi(numV), false, true, true);

            for (int i = 0; i < stoi(numCluster); i++)
            {

                clusters[i] = new Cluster(0, stof(clusterCapMax), G, clusterType);
            }

            for (int i = 0; i < stoi(numV); i++)
            {
                getline(arq, line);
                string pesoNode = line;
                G->inserirNo(i, stof(pesoNode));
            }

            getline(arq, line);
            const char *matrix = line.c_str();
            int n = stoi(numV);
            int **M = new int *[n];

            for (int i = 0; i < n; i++)
                M[i] = new int[n];

            int i = 0, j = 0, percorre = 0;
            char c;
            string s = "";
            char m = matrix[percorre];

            while (j < n)
            {
                m = matrix[percorre];
                percorre++;
                if (m != ' ')
                    s += m;
                else
                {
                    M[j][i] = stof(s);
                    // if(s != "0"){
                    //     cout << "inserindo: " << s << " em: " << j << "," << i << endl;
                    // }
                    s = "";
                }
                if (i < n && s == "")
                    i++;
                else if (i == n)
                {
                    i = 0;
                    j++;
                }
            }

            for (int k = 0; k < n; k++)
                for (int l = 0; l < n; l++)
                {
                    if (M[k][l] != 0)
                    {
                        if (k != l)
                            G->inserirArco(k, l, M[k][l]);
                    }
                }
            return G;
        }

        if (instance == "RanReal240" || instance == "RanReal480" || instance == "RanReal960" || instance == "Sparse82")
        {
            getline(arq, line);
            const char *linha = line.c_str();
            int aux = 0;
            char c = linha[aux];

            string s = "";
            bool nC = false;         // indica se pegou o numero de clusters
            bool cT = false;         // indica se pegou o tipo do cluseter
            bool cLmin = false;      // indica se pegou o limite inferior do cluster
            bool cLmax = false;      // indica se pegou o limite superior do cluster
            bool w = false;          // indica que alcançou a letra W para começar a ler os pesos dos nós
            int id = 0;              // ids dos nos
            int contadorCluster = 0; // auxiliar para indicar o cluster que ele está atribuindo os parâmetros

            while (c != ' ')
            {
                c = linha[aux];
                aux++;
                s += c;
            }

            int n = stoi(s);

            Grafo *G = new Grafo(n, false, true, true);

            c = linha[aux + 1];
            s = "";

            while (c != '\0')
            {
                c = linha[aux];
                aux++;

                if (!nC)
                {
                    if (c != ' ')
                        s += c;
                    else
                    {
                        numCluster = s;
                        nC = true;
                        s = "";
                        clusters = new Cluster *[stoi(numCluster)];
                    }
                }

                else if (nC && !cT)
                {
                    if (c != ' ')
                        s += c;
                    else
                    {
                        clusterType = s;
                        cT = true;
                        s = "";
                    }
                }

                else if (cT && !cLmin)
                {
                    if (c != ' ')
                        s += c;

                    else
                    {
                        clusterCapmin = s;
                        cLmin = true;
                        cLmax = false;
                        s = "";
                    }
                }

                else if (cLmin && !cLmax && c != 'W')
                {
                    if (c != ' ')
                        s += c;
                    else
                    {
                        clusterCapMax = s;
                        cLmax = true;
                        cLmin = false;
                        s = "";
                        clusters[contadorCluster] = new Cluster(stoi(clusterCapmin), stof(clusterCapMax), G, clusterType);
                        contadorCluster++;
                    }
                }

                if (c == 'W')
                {
                    cLmax = true;
                    cLmin = true;
                    w = true;
                    s = "";
                    aux++;
                }
                if (w == true && c != 'W')
                {
                    if (isdigit(c))
                        s += c;
                    else
                    {
                        if (c == ' ' && s != "")
                        {
                            G->inserirNo(id, stof(s));
                            s = "";
                            id++;
                        }
                    }
                }
            }

            if (s != "")
            {
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
void escritaArquivo(string pathOut, Grafo *G)
{

    ofstream arqSaida(pathOut);

    if (arqSaida.is_open())
    {
        arqSaida << to_string(G->getOrdem()) + "\n";

        if (G->ehDir())
        {
            if (G->ehPondNode())
            {
                No *noInicial = G->getNoInicial();
                while (noInicial != NULL)
                {
                    arqSaida << to_string(noInicial->getId()) + " " + to_string(noInicial->getGrauEntrada()) + to_string(noInicial->getGrauSaida()) + "\n";
                    noInicial = noInicial->getProx();
                }
            }

            if (G->ehPondAr())
            {
                No *no = G->getNoInicial();
                while (no != NULL)
                {
                    Arco *arco = no->getAdjacentes();
                    while (arco != NULL)
                    {
                        arqSaida << to_string(no->getId()) + " " + to_string(arco->getNodeDest()) + " " + to_string(arco->getPeso()) + "\n";
                        arco = arco->getProx();
                    }
                    no = no->getProx();
                }
            }

            else
            {
                No *no = G->getNoInicial();
                while (no != NULL)
                {
                    Arco *arco = no->getAdjacentes();
                    while (arco != NULL)
                    {
                        arqSaida << to_string(no->getId()) + " " + to_string(arco->getNodeDest()) + "\n";
                        arco = arco->getProx();
                    }
                    no = no->getProx();
                }
            }
        }
        else
        {
            if (G->ehPondNode())
            {
                No *noInicial = G->getNoInicial();
                while (noInicial != NULL)
                {
                    arqSaida << to_string(noInicial->getId()) + " " + to_string(noInicial->getGrauEntrada()) + "\n";
                    noInicial = noInicial->getProx();
                }
            }

            if (G->ehPondAr())
            {
                No *no = G->getNoInicial();
                int *visitados = new int[G->getOrdem()];
                int v = 1;
                bool visitado = false;
                while (no != NULL)
                {
                    Arco *arco = no->getAdjacentes();
                    visitados[v - 1] = no->getId();
                    while (arco != NULL)
                    {
                        for (int i = 0; i < v; i++)
                        {
                            if (arco->getNodeDest() == visitados[i])
                                visitado = true;
                        }
                        if (visitado)
                            arqSaida << to_string(no->getId()) + " " + to_string(arco->getNodeDest()) + " " + to_string(arco->getPeso()) + "\n";
                        arco = arco->getProx();
                        visitado = false;
                    }
                    no = no->getProx();
                    v++;
                    delete arco;
                }

                delete[] visitados;
            }

            else
            {
                No *no = G->getNoInicial();
                int *visitados = new int[G->getOrdem()];
                int v = 1;
                bool visitado = false;
                while (no != NULL)
                {
                    Arco *arco = no->getAdjacentes();
                    visitados[v - 1] = no->getId();
                    while (arco != NULL)
                    {
                        for (int i = 0; i < v; i++)
                        {
                            if (arco->getNodeDest() == visitados[i])
                                visitado = true;
                        }
                        if (visitado)
                            arqSaida << to_string(no->getId()) + " " + to_string(arco->getNodeDest()) + "\n";
                        arco = arco->getProx();
                        visitado = false;
                    }
                    no = no->getProx();
                    v++;
                    delete arco;
                }

                delete[] visitados;
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
    string ehDir, ehPondNode, ehPondAr, instance, algoritmo;

    float alfaRandomizado; // Para o randomizado

    // Parametros para randomizado reativo
    int num_iteracoes;
    int bloco;
    int num_alfas = 5;
    float alfas[] = {0.05, 0.10, 0.15, 0.30, 0.5};

    if (argc < 8)
    {
        cout << "Param 0: " << argv[0] << endl;
        cout << "Param 1: " << argv[1] << endl;

        cout << "Entrada inválida: " << argc << " parametros..." << endl;
        return 0;
    }

    algoritmo = argv[7];

    if (algoritmo != "G" && algoritmo != "GRA" && algoritmo != "GRAR")
    {
        cout << "Algoritmo inválido: " << algoritmo << endl;
        cout << "Informe uma das seguintes opções: " << endl;
        cout << "G - Algoritmo guloso" << endl;
        cout << "GRA - Algoritmo guloso randomizado adaptativo" << endl;
        cout << "GRAR - Algoritmo guloso randomizado adaptativo reativo" << endl;
        return 0;
    }

    if (algoritmo == "GRA")
    {
        // Necessário passar alfa como ultimo parametro...
        if (argc != 10)
        {
            cout << "Informar o número de iterações e  valor do alfa para o guloso randomizado adaptativo..." << endl;
            return 0;
        }
        num_iteracoes = std::stoi(argv[8]);
        alfaRandomizado = std::atof(argv[9]);
    }

    if (algoritmo == "GRAR")
    {
        if (argc != 10)
        {
            cout << "Informar o numero de iterações e o valor do bloco do randomizado reativo" << endl;
            return 0;
        }

        num_iteracoes = std::stoi(argv[8]);
        bloco = std::stoi(argv[9]);
    }

    pathIn = argv[1];
    pathOut = argv[2];
    instance = argv[3];
    ehDir = argv[4];
    ehPondAr = argv[5];
    ehPondNode = argv[6];

    string nomeArquivo = getFileName(pathIn);

    Grafo *G = leituraArquivoFase2(pathIn, instance, "0", "1", "1");

    int num_clusters;
    float min;
    float max;

    if (nomeArquivo == "20_5_270001")
    {
        num_clusters = 5;
        min = 0;
        max = 106.704002;
    }
    else if (nomeArquivo == "20_10_270001")
    {
        num_clusters = 10;
        min = 0;
        max = 53.351898;
    }
    else if (nomeArquivo == "30_5_270003")
    {
        num_clusters = 5;
        min = 0;
        max = 172.378006;
    }
    else if (nomeArquivo == "RanReal240_01.txt")
    {
        num_clusters = 12;
        min = 75;
        max = 125;
    }
    else if (nomeArquivo == "RanReal240_04.txt")
    {
        num_clusters = 12;
        min = 75;
        max = 125;
    }
    else if (nomeArquivo == "RanReal240_07.txt")
    {
        num_clusters = 12;
        min = 75;
        max = 125;
    }
    else if (nomeArquivo == "RanReal480_01.txt")
    {
        num_clusters = 20;
        min = 100;
        max = 150;
    }
    else if (nomeArquivo == "RanReal480_04.txt")
    {
        num_clusters = 20;
        min = 100;
        max = 150;
    }
    else if (nomeArquivo == "RanReal960_01.30.txt")
    {
        num_clusters = 30;
        min = 120;
        max = 180;
    }
    else if (nomeArquivo == "Sparse82_02.txt")
    {
        num_clusters = 8;
        min = 25;
        max = 75;
    }
    else
    {
        cout << "Nome instancia inválida: " << nomeArquivo << endl;
        return 0;
    }

    string fileMetricasG = "../resultados/metricasG.csv";
    string fileMetricasGRA = "../resultados/metricasGRA.csv";
    string fileMetricasGRAR = "../resultados/metricasGRAR.csv";

    if (algoritmo == "G")
    {
        std::ifstream arquivoExistente(fileMetricasG);
        if (!arquivoExistente.good())
        {
            ofstream tempFile(fileMetricasG, ios::app);
            tempFile << "Instância,Semente,Solução,Tempo" << endl;
            tempFile.close();
        }

        ofstream metricas(fileMetricasG, ios::app);
        metricas << nomeArquivo;

        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();

        guloso(G, clusters, num_clusters, min, max, pathOut, metricas);

        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        metricas << "," << elapsed_seconds.count();
        metricas << endl;
        metricas.close();
    }
    else if (algoritmo == "GRA")
    {
        std::ifstream arquivoExistente(fileMetricasGRA);
        if (!arquivoExistente.good())
        {
            ofstream tempFile(fileMetricasGRA, ios::app);
            tempFile << "Instância,Semente,Solução,Tempo,Iteraçoes,Alfa" << endl;
            tempFile.close();
        }

        ofstream metricas(fileMetricasGRA, ios::app);
        metricas << nomeArquivo;

        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();

        gulosoRandomizado(G, clusters, num_clusters, min, max, alfaRandomizado, num_iteracoes, pathOut, metricas);

        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        metricas << "," << elapsed_seconds.count();
        metricas << "," << num_iteracoes;
        metricas << "," << alfaRandomizado;
        metricas << endl;
        metricas.close();
    }
    else if (algoritmo == "GRAR")
    {
        std::ifstream arquivoExistente(fileMetricasGRAR);
        if (!arquivoExistente.good())
        {
            ofstream tempFile(fileMetricasGRAR, ios::app);
            tempFile << "Instância,Semente,Solução,melhorAlfa,Tempo,Iteraçoes,Bloco" << endl;
            tempFile.close();
        }

        ofstream metricas(fileMetricasGRAR, ios::app);
        metricas << nomeArquivo;

        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();

        gulosoRandomizadoReativo(G, clusters, num_clusters, min, max, alfas, num_iteracoes, bloco, num_alfas, pathOut, metricas);

        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        metricas << "," << elapsed_seconds.count();
        metricas << "," << num_iteracoes << "," << bloco;
        metricas << endl;
        metricas.close();
    }

    // Descomente as linhas para testar os algoritmos para cada instância do seu desejo.

    /*Algoritmo Guloso*/

    // guloso(G, clusters, 5, 0, 106.704002, pathOut); //20_5_270001  cmake .. && make && ./Grupo_1 ../instancias/20_5_270001 ../saida.txt Handover 0 1 0

    // guloso(G, clusters, 10, 0, 53.351898, pathOut); //20_10_270001 cmake .. && make && ./Grupo_1 ../instancias/20_10_270001 ../saida.txt Handover 0 1 0

    // guloso(G, clusters, 5, 0, 172.378006, pathOut);//30_5_270003 cmake .. && make && ./Grupo_1 ../instancias/30_5_270003 ../saida.txt Handover 0 1 0

    // guloso(G, clusters, 8, 25, 75, pathOut);//Sparse_82_02 cmake .. && make && ./Grupo_1 ../instancias/Sparse82_02.txt ../saida.txt Sparse82 0 1 0

    // guloso(G, clusters, 12, 75, 125, pathOut);//RanReal240_01 cmake .. && make && ./Grupo_1 ../instancias/RanReal240_01.txt ../saida.txt RanReal240 0 1 0

    // guloso(G, clusters, 20, 100, 150, pathOut);//RanReal480_01 cmake .. && make && ./Grupo_1 ../instancias/RanReal480_01.txt ../saida.txt RanReal480 0 1 0

    // guloso(G, clusters, 30, 120, 180, pathOut);//RanReal960_01 cmake .. && make && ./Grupo_1 ../instancias/RanReal960_01.30.txt ../saida.txt RanReal960 0 1 0

    /*Algoritmo Guloso Randomizado*/

    // gulosoRandomizado(G, clusters, 5, 0, 106.704002, 0.05, 500, pathOut);//20_5_270001  cmake .. && make && ./Grupo_1 ../instancias/20_5_270001 ../saida.txt Handover 0 1 0

    // gulosoRandomizado(G, clusters, 10, 0, 53.351898, 0.35, 500, pathOut);//20_10_270001 cmake .. && make && ./Grupo_1 ../instancias/20_10_270001 ../saida.txt Handover 0 1 0

    // gulosoRandomizado(G, clusters, 5, 0, 172.378006, 0.05, 500, pathOut);//30_5_270003 cmake .. && make && ./Grupo_1 ../instancias/30_5_270003 ../saida.txt Handover 0 1 0

    // gulosoRandomizado(G, clusters, 8, 25, 75, 0.05, 500, pathOut);//Sparse_82_02 cmake .. && make && ./Grupo_1 ../instancias/Sparse82_02.txt ../saida.txt Sparse82 0 1 0

    // gulosoRandomizado(G, clusters, 12, 75, 125, 0.05, 500, pathOut);//RanReal240_01 cmake .. && make && ./Grupo_1 ../instancias/RanReal240_01.txt ../saida.txt RanReal240 0 1 0

    // gulosoRandomizado(G, clusters, 20, 100, 150, 0.05, 500, pathOut);//RanReal480_01 cmake .. && make && ./Grupo_1 ../instancias/RanReal480_01.txt ../saida.txt RanReal480 0 1 0

    // gulosoRandomizado(G, clusters, 30, 120, 180, 0.05, 500, pathOut);//RanReal960_01 cmake .. && make && ./Grupo_1 ../instancias/RanReal960_01.30.txt ../saida.txt RanReal960 0 1 0

    /*Algoritmo Guloso Randomizado Adaptativo Reativo*/

    // alfa = 1 ==> vai sortear qualquer indice do vetor de candidatos
    // alfa = 0 ==> vai pegar sempre o primeiro do vetor de candidatos (guloso)

    /*Vetor com alfas escolhidos para teste*/
    // float alfas[] = {0.08, 0.18, 0.28, 0.37, 0.53,0.22, 0.64, 0.89, 0.77, 0.45};

    // gulosoRandomizadoReativo(G, clusters, 5, 0, 106.704002, alfas, 500, 50, 5, pathOut);//20_5_270001  cmake .. && make && ./Grupo_1 ../instancias/20_5_270001 ../saida.txt Handover 0 1 0

    // gulosoRandomizadoReativo(G, clusters, 10, 0, 53.351898, alfas, 500, 50, 5, pathOut);//20_10_270001 cmake .. && make && ./Grupo_1 ../instancias/20_10_270001 ../saida.txt Handover 0 1 0

    // gulosoRandomizadoReativo(G, clusters, 5, 0, 172.378006, alfas, 500, 50, 5, pathOut);//30_5_270003 cmake .. && make && ./Grupo_1 ../instancias/30_5_270003 ../saida.txt Handover 0 1 0

    // gulosoRandomizadoReativo(G, clusters, 8, 25, 75, alfas, 500, 50, 5, pathOut);//Sparse_82_02 cmake .. && make && ./Grupo_1 ../instancias/Sparse82_02.txt ../saida.txt Sparse82 0 1 0

    // gulosoRandomizadoReativo(G, clusters, 12, 75, 125, alfas, 500, 50, 5, pathOut);//RanReal240_01 cmake .. && make && ./Grupo_1 ../instancias/RanReal240_01.txt ../saida.txt RanReal240 0 1 0

    // gulosoRandomizadoReativo(G, clusters, 20, 100, 150, alfas, 500, 50, 5, pathOut);//RanReal480_01 cmake .. && make && ./Grupo_1 ../instancias/RanReal480_01.txt ../saida.txt RanReal480 0 1 0

    // gulosoRandomizadoReativo(G, clusters, 30, 120, 180, alfas, 500, 50, 5, pathOut);//RanReal960_01 cmake .. && make && ./Grupo_1 ../instancias/RanReal960_01.30.txt ../saida.txt RanReal960 0 1 0

    return 0;
}