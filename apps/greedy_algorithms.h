#ifndef GREEDY_ALGORITHMS_CPP
#define GREEDY_ALGORITHMS_CPP

#include "../include/Grafo.h"
#include "../include/Cluster.h"
#include <iostream>
#include <forward_list>
#include <queue>
#include <tuple>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <sstream>
#include <fstream>
#include <cmath>
using namespace std;

// Estrutura auxiliar para ajudar a controlar a questão dos graus dos nós a serem inseridos inicialmente nos clusters.
typedef struct
{
    int idNo;
    int grau;
    float peso;
} NoUtils;

typedef struct
{
    float melhorSolucao;
    int *pesodeCadaCluster;
} BestSol;


bool solucao(Cluster **clusters, int num_clusters)
{
    for (int i = 0; i < num_clusters; i++)
    {
        if (clusters[i]->getSumVertices() > clusters[i]->getMax() || clusters[i]->getSumVertices()<clusters[i]->getMin()){
            return false;
        }
    }
    return true;
}

void escreveNosCluster(Cluster **clusters, int num_clusters, string pathOut){
   if(solucao(clusters, num_clusters))
   {
    ofstream nosCluster(pathOut);
   float solucao = 0;
   for(int i=0;i<num_clusters;i++){
       nosCluster <<"Cluster "<<i<<endl;
       std::forward_list<int> inseridos = clusters[i]->getInseridos();
       nosCluster<<"\tSoma Vértices: "<<clusters[i]->getSumVertices()<<endl;
       nosCluster<<"\tSoma Arestas: "<<clusters[i]->getSumArestas()<<endl;
       nosCluster << "\tNós adicionados no cluster\n";
       solucao+=clusters[i]->getSumArestas();
       for (std::forward_list<int>::iterator it = inseridos.begin(); it != inseridos.end(); it++)
       {
           nosCluster<<"\t"<<(*it);  
       }
       nosCluster<<endl;
   }
   nosCluster<<"-------------- SOLUÇÃO: "<<solucao<<" --------------"<<endl;
   nosCluster.close();
   }
   else{
        cout << "Nenhuma solução foi encontrada\n";
   }
}


void criarArquivoDotClusters(Cluster **clusters,int num_clusters, const string &nomeArquivo)
{
   ofstream arquivo(nomeArquivo);


   if (!arquivo.is_open())
   {
       cout << "Erro ao criar o arquivo " << nomeArquivo << endl;
       return;
   }


   // Escrever cabeçalho do arquivo
   arquivo << "graph Grafo {" << endl; // Utilizamos "graph" em vez de "digraph"


   for(int i=0;i<num_clusters;i++){
       std::forward_list<int> inseridos = clusters[i]->getInseridos();
       for (std::forward_list<int>::iterator it = inseridos.begin(); it != inseridos.end(); it++)
       {
             arquivo << "    " << (*it) << " [label=\"" << (*it) << "\"];";
       }
    
   }


   for (int i = 0; i < num_clusters; i++)
   {
       Grafo *grafo = clusters[i]->getGrafo();
       // Escrever arcos (arestas)
       No *noAtual = grafo->getNoInicial();
       while (noAtual != nullptr)
       {
             Arco *arcoAtual = noAtual->getAdjacentes();
             while (arcoAtual != nullptr)
             {
               // Verificamos se o arco já foi visitado antes de escrever a aresta correspondente
               // Para um grafo não direcionado, consideramos apenas um sentido para cada par de nós
               if (noAtual->getId() < arcoAtual->getNodeDest())
               {
                   arquivo << "    " << noAtual->getId() << " -- " << grafo->findNoById(arcoAtual->getNodeDest())->getId();


                   // Se a aresta (arco) tiver peso, escrever o atributo "peso"
                   if (grafo->ehPondAr())
                   {
                       arquivo << " [label=" << arcoAtual->getPeso() << ", weight=" << arcoAtual->getPeso() << "]";
                   }


                   arquivo << ";" << endl;
               }


               arcoAtual = arcoAtual->getProx();
             }


             noAtual = noAtual->getProx();
       }
   }


   // Fechar arquivo
   arquivo << "}" << endl;
   arquivo.close();


   cout << "Arquivo " << nomeArquivo << " criado com sucesso!" << endl;
}


bool compareNos(NoUtils a, NoUtils b)
{
    return a.peso <= b.peso;
}

void inicializaMatriz(Grafo *g, int **matriz)
{
    No *noOrigem;
    Arco *arco;
    for (int i = 0; i < g->getOrdem(); i++)
    {

        noOrigem = g->findNoById(i);

        arco = noOrigem->getAdjacentes();

        while (arco != nullptr)
        {

            matriz[i][arco->getNodeDest()] = arco->getPeso();

            arco = arco->getProx();
        }
    }
}

// Compara as contribuições dos candidatos.
bool compareCandidatos(std::tuple<int, int, float, int> a, std::tuple<int, int, float, int> b)
{
    return std::get<2>(a) > std::get<2>(b);
}

int clusterViavel(Cluster **clusters, int num_clusters, float max, float pesoNo)
{
    int j = rand() % num_clusters;
    while (clusters[j]->getSumVertices() + pesoNo > max)
    {
        j = rand() % num_clusters;
    }
    return j;
}

std::forward_list<std::tuple<int, int, float, int>> rankeiaCandidatos(Grafo *g, Cluster **clusters, int *nosInseridos, int **matriz, int num_clusters, float min, float max)
{

    std::forward_list<std::tuple<int, int, float, int>> candidatos;
    float auxContribuicao;
    float melhorContribuicao;
    int melhorCluster;

    // Para cada nó
    for (int i = 0; i < g->getOrdem(); i++)
    {
        melhorContribuicao = 0;
        melhorCluster = 0;
        // Se não estiver em algum cluster
        if (nosInseridos[i] == -1)
        {

            // Para cada cluster
            No *analisado = g->findNoById(i); //-> Nó que está sendo analisado no momento

            for (int j = 0; j < num_clusters; j++)
            {

                // Laço para computar a contribuição do nó em cluster da lista, caso a inserção do nó não viole as restrições do problema.

                // Avaliação de Restrição
                if (clusters[j]->getSumVertices() + analisado->getPeso() < min)
                {
                    auxContribuicao = 0;
                    std::forward_list<int> inseridos = clusters[j]->getInseridos();

                    auxContribuicao += matriz[inseridos.front()][i];
                    
                }
                else
                {
                    if (clusters[j]->getSumVertices() + analisado->getPeso() <= max)
                    {

                        auxContribuicao = 0;
                        std::forward_list<int> inseridos = clusters[j]->getInseridos();

                        auxContribuicao += matriz[inseridos.front()][i];

                        float residuo = max - clusters[j]->getSumVertices() - analisado->getPeso();
                        auxContribuicao = (auxContribuicao * residuo) / analisado->getPeso();
                    }
                }

                if (auxContribuicao >= melhorContribuicao)
                {
                    melhorContribuicao = auxContribuicao;
                    melhorCluster = j;
                }
            }

            if (melhorContribuicao == 0)
            {
                melhorCluster = clusterViavel(clusters, num_clusters, max, analisado->getPeso());
            }

            //* Tupla consiste na quadrupla: id do nó que ainda não foi inserido em nenhum cluster, id do cluster, contribuição para o cluster, peso do nó.
            std::tuple<int, int, float, int> tup;
            tup = std::make_tuple(i, melhorCluster, melhorContribuicao, analisado->getPeso());
            candidatos.push_front(tup);
        }
    }

    candidatos.sort(compareCandidatos);
    return candidatos;
}

std::forward_list<std::tuple<int, int, float, int>> atualizaCandidatos(Grafo *g, Cluster **clusters, int *nosInseridos, int num_clusters, float min, float max, std::forward_list<std::tuple<int, int, float, int>> candidatos, int **matriz, int ultimoInserido, int idClusterUltimoInserido)
{

    // Atualizar a lista de candidatos em relação a esse cara que foi o último a ser inserido.
    std::forward_list<std::tuple<int, int, float, int>> newCandidatos;

    for (std::forward_list<std::tuple<int, int, float, int>>::iterator it = candidatos.begin(); it != candidatos.end(); it++)
    {

        // último a ser inserido não pode continuar na lista de candidatos.
        if (std::get<0>(*it) != ultimoInserido)
        {
            if (std::get<1>(*it) != idClusterUltimoInserido)
            {
                // Esse nó pode continuar pois não alterou nada pra ele a última inserção.
                newCandidatos.push_front(*it);
            }
            else
            {
                No *analisado = g->findNoById(std::get<0>(*it));
                // Tem relação com o último cluster que foi inserido.

                if (clusters[std::get<1>(*it)]->getSumVertices() + analisado->getPeso() < min)
                {

                    std::get<2>(*it) += matriz[std::get<0>(*it)][ultimoInserido];

                    newCandidatos.push_front(*it);
                }
                else if (clusters[std::get<1>(*it)]->getSumVertices() + analisado->getPeso() <= max)
                {
                    std::get<2>(*it) += matriz[std::get<0>(*it)][ultimoInserido]; // Atualiza a contribuição contabilizando a aresta entre esse avaliddo e o cara que foi inserido.

                    float residuo = max - clusters[std::get<1>(*it)]->getSumVertices() - analisado->getPeso();

                    std::get<2>(*it) = (std::get<2>(*it) * residuo) / analisado->getPeso();

                    newCandidatos.push_front(*it);
                }
            }
        }
    }

    newCandidatos.sort(compareCandidatos);
    return newCandidatos;
}

bool comparePesos(std::tuple<int, int, float, int> a, std::tuple<int, int, float, int> b)
{
    return std::get<3>(a) <= std::get<3>(b);
}

/*
    '''Dado um grafo g e um vetor com os clusters, essa função será capaz de devolver em um arquivo .txt uma solução aproximada para o problema de clusterização capacitada. Nesse arquivo, estarão contidos a soma dos pesos das arestas de cada cluster, nós que cada cluster possui, e soma total das arestas dos clusters (Qualidade da solução).'''

    ? g: Grafo que será analisado
    ? param clusters: Vetor de clusters instanciado durante a leitura do arquivo de instâncias do problema
    ? param num_clusters: Inteiro que representa o tamanho do vetor de clusters
    ? param min, max: Inteiros que representam o valor máximo e mínimo do intervalo dos clusters
*/
void guloso(Grafo *g, Cluster **clusters, int num_clusters, float min, float max, string pathOut)
{

    time_t seed = time(NULL);

    cout << "VALOR DA SEMENTE: " << seed << endl;

    srand(seed);

    int **matriz = new int *[g->getOrdem()];

    std::tuple<int, int, float, int> top;

    std::forward_list<std::tuple<int, int, float, int>> candidatos;

    // Matriz para saber o peso da aresta entre os nós i e j
    for (int i = 0; i < g->getOrdem(); i++)
    {
        matriz[i] = new int[g->getOrdem()];
        for (int j = 0; j < g->getOrdem(); j++)
        {
            matriz[i][j] = 0;
        }
    }

    int *nosInseridos = new int[g->getOrdem()]; // guarda em cada posição o cluster que o nó foi inserido
    for (int i = 0; i < g->getOrdem(); i++)
    {
        nosInseridos[i] = -1; // Flag -1 indica que ainda não foi inserido
    }

    inicializaMatriz(g, matriz);

    // Inicialização dos num_clusters com nós aleatórios.
    int cont;
    for (cont = 0; cont < num_clusters; cont++)
    {
        int idRandom = rand() % g->getOrdem();
        while (nosInseridos[idRandom] != -1)
        {
            idRandom = rand() % g->getOrdem();
        }
        clusters[cont]->inserirNoCluster(g->findNoById(idRandom));
        nosInseridos[idRandom] = cont;
    }

    float qualidadeSolucao = 0;

    // Cont vai chegar aqui valendo num_clusters

    candidatos = rankeiaCandidatos(g, clusters, nosInseridos, matriz, num_clusters, min, max);

    while (cont < g->getOrdem() && !candidatos.empty())
    {

        top = candidatos.front();

        clusters[std::get<1>(top)]->inserirNoCluster(g->findNoById(std::get<0>(top)));

        nosInseridos[std::get<0>(top)] = std::get<1>(top);

        candidatos = atualizaCandidatos(g, clusters, nosInseridos, num_clusters, min, max, candidatos, matriz, std::get<0>(top), std::get<1>(top));

        cont += 1;
    }

    escreveNosCluster(clusters, num_clusters, pathOut);
}

int randomRange(int num_candidatos, float alfa)
{
    int mod = ceil(alfa * num_candidatos);
    return rand() % mod;
}

// Função para copiar o cluster.
Cluster **copiaClusters(Cluster **cluster, int num_clusters, Grafo *g)
{
    Cluster **newCluster = new Cluster *[num_clusters];
    for (int i = 0; i < num_clusters; i++)
    {
        std::forward_list<int> nosInseridos = cluster[i]->getInseridos();
        newCluster[i] = new Cluster(cluster[i]->getMin(), cluster[i]->getMax(), g, cluster[i]->getType());

        for (std::forward_list<int>::iterator it = nosInseridos.begin(); it != nosInseridos.end(); it++)
        {
            newCluster[i]->inserirNoCluster(g->findNoById((*it)));
        }
    }
    return newCluster;
}

/*
    Soma o peso das arestas de cada cluster
*/
float calculaQualidadeSolucao(Cluster **cluster, int num_clusters)
{
    float qualidade = 0;
    for (int i = 0; i < num_clusters; i++)
    {
        qualidade += cluster[i]->getSumArestas();
    }
    return qualidade;
}

/*
    '''Dado um grafo g e um vetor com os clusters, essa função será capaz de devolver em um arquivo .txt uma solução aproximada para o problema de clusterização capacitada. Nesse arquivo, estarão contidos a soma dos pesos das arestas de cada cluster, nós que cada cluster possui, e soma total das arestas dos clusters (Qualidade da solução).'''

    ? g: Grafo que será analisado
    ? param clusters: Vetor de clusters instanciado durante a leitura do arquivo de instâncias do problema
    ? param num_clusters: Inteiro que representa o tamanho do vetor de clusters
    ? param min, max: Inteiros que representam o valor máximo e mínimo do intervalo dos clusters
*/
void gulosoRandomizado(Grafo *g, Cluster **clusters, int num_clusters, float min, float max, float alfa, int num_iteracoes, string pathOut)
{

    time_t seed = time(NULL);
    srand(seed);
    cout << "VALOR DA SEMENTE: " << seed << endl;

    // Matriz para saber o peso da aresta entre os nós i e j
    int **matriz = new int *[g->getOrdem()];

    std::forward_list<std::tuple<int, int, float, int>> candidatos;

    for (int i = 0; i < g->getOrdem(); i++)
    {
        matriz[i] = new int[g->getOrdem()];
        for (int j = 0; j < g->getOrdem(); j++)
        {
            matriz[i][j] = 0;
        }
    }
    inicializaMatriz(g, matriz);

    int *inicializacaoIndices = new int[num_clusters];

    int *nosInseridos = new int[g->getOrdem()];
    for (int i = 0; i < g->getOrdem(); i++)
    {
        nosInseridos[i] = -1; // Flag -1 indica que ainda não foi inserido
    }

    for (int i = 0; i < num_clusters; i++)
    {
        int idRandom = rand() % g->getOrdem(); // Id de um nó
        while (nosInseridos[idRandom] != -1)
        {
            idRandom = rand() % g->getOrdem();
        }
        inicializacaoIndices[i] = idRandom; // Para cada indice que representa o indice de um cluster, nessa posição estará o valor de qual nó que o inicializará.
        nosInseridos[idRandom] = i;         // Na posição idRandom que é o id de um nó eu taco qual cluster que ele vai entrar
    }

    int k = 0;

    Cluster **melhorSolucao = nullptr;

    while (k < num_iteracoes)
    {

        // Inicialização dos num_clusters com nós aleatórios.
        int cont;
        for (cont = 0; cont < num_clusters; cont++)
        {
            clusters[cont]->inserirNoCluster(g->findNoById(inicializacaoIndices[cont]));
            nosInseridos[inicializacaoIndices[cont]] = cont; // Reinicializar de novo
        }

        // Cont vai chegar aqui valendo num_clusters

        candidatos = rankeiaCandidatos(g, clusters, nosInseridos, matriz, num_clusters, min, max);

        while (cont < g->getOrdem() && !candidatos.empty())
        {
            int index = randomRange(std::distance(candidatos.begin(), candidatos.end()), alfa);
            std::tuple<int, int, float, int> top;
            int contador = 0;

            for (std::forward_list<std::tuple<int, int, float, int>>::iterator it = candidatos.begin(); it != candidatos.end(); it++)
            {
                if (index == contador)
                {
                    top = (*it);
                    break;
                }
                else
                {
                    contador++;
                }
            }

            clusters[std::get<1>(top)]->inserirNoCluster(g->findNoById(std::get<0>(top)));

            nosInseridos[std::get<0>(top)] = std::get<1>(top);

            candidatos = atualizaCandidatos(g, clusters, nosInseridos, num_clusters, min, max, candidatos, matriz, std::get<0>(top), std::get<1>(top));

            cont += 1;
        }

        if (melhorSolucao == nullptr && solucao(clusters, num_clusters))
        {
            melhorSolucao = copiaClusters(clusters, num_clusters, g);
        }
        else
        {
            if (solucao(clusters, num_clusters) && calculaQualidadeSolucao(clusters, num_clusters) > calculaQualidadeSolucao(melhorSolucao, num_clusters))
            {
                
                melhorSolucao = copiaClusters(clusters, num_clusters, g);
            }
        }

        k++;

        // Zera todos os clusters para a próxima iteração.
        for (int i = 0; i < g->getOrdem(); i++)
        {
            if (i < num_clusters)
            {
                clusters[i]->zeraCluster(g);
            }
            nosInseridos[i] = -1; // Resetar
        }
    }

    if (melhorSolucao == nullptr)
    {
        cout << "Solução não encontrada..." << endl;
    }else{
        escreveNosCluster(melhorSolucao, num_clusters, pathOut);
    }   
}

void inicializaVetores(float *P, float *M, int *escolhasAlfa, float *somaSolucoes, BestSol *solBest, int m)
{
    for (int i = 0; i < m; i++)
    {

        P[i] = 1.0 / m; // inicia todos com a mesma probabilidade
        M[i] = 0;
        escolhasAlfa[i] = 0;
        somaSolucoes[i] = 0;
        solBest[i].melhorSolucao = 0;
    }
}

void atualizaProbabilidades(float *P, float *medias, float *alfa, BestSol solBest, int m)
{
    float q[m];
    float soma = 0;
    int cont = 0;
    for (int i = 0; i < m; i++)
    {
        q[i] = 0;
        if (medias[i] != 0)
        {
            // q[i] = solBest[i].melhorSolucao / medias[i];
            q[i] = solBest.melhorSolucao / medias[i];
            q[i] = pow(q[i], 100);
            soma += q[i];
        }else{
            cont++;
        }
    }

    for (int i = 0; i < m; i++)
    {
        // apenas se calculou o q[i], ou seja, se o vetor de médias é diferente de 0
        // muda a probabilidade de todos os alfas
        if (q[i] != 0)
        {
            // cout <<"atualizou"<<endl;
            P[i] = ((q[i] / soma)-0.01);
            if(P[i]<=0)
                P[i] = 0.01;
        }else{
            P[i] = 0.01;
        }
    }
}

int escolheAlfa(float *P, int tamVetAlfa)
{
    float k = rand() % 101;
    float soma = 0;
    int i = 0;
    for (i = 0; i < tamVetAlfa; i++)
    {
        soma += (P[i] * 100);
        if (soma >= k)
        {
            return i;
        }
    }

    return i;

}

void atualizaMedias(float *M, float s, float *alfas)
{
}

/*
    '''Dado um grafo g e um vetor com os clusters, essa função será capaz de devolver em um arquivo .txt uma solução aproximada para o problema de clusterização capacitada. Nesse arquivo, estarão contidos a soma dos pesos das arestas de cada cluster, nós que cada cluster possui, e soma total das arestas dos clusters (Qualidade da solução).'''

    ? g: Grafo que será analisado
    ? param clusters: Vetor de clusters instanciado durante a leitura do arquivo de instâncias do problema
    ? param num_clusters: Inteiro que representa o tamanho do vetor de clusters
    ? param min, max: Inteiros que representam o valor máximo e mínimo do intervalo dos clusters
*/
void gulosoRandomizadoReativo(Grafo *g, Cluster **clusters, int num_clusters, float min, float max, float *alfas, int num_iteracoes, int bloco, int m, string pathOut)
{
    time_t seed = time(NULL);
    srand(seed);
    cout << "VALOR DA SEMENTE: " << seed << endl;

    // Matriz para saber o peso da aresta entre os nós i e j
    int **matriz = new int *[g->getOrdem()];

    float P[m];            // probabilidades dos alfas
    float somaSolucoes[m]; // soma acumulada das soluções
    float medias[m];       // Média para cada alfa
    int escolhasAlfa[m];   // armazena em cada posição quantas vezes o alfa i foi escolhido => fazer a média posteriormente
    BestSol solBest[m];
    BestSol melhorSolucaoGeral;
    melhorSolucaoGeral.melhorSolucao = 0;

    inicializaVetores(P, medias, escolhasAlfa, somaSolucoes, solBest, m);

    std::forward_list<std::tuple<int, int, float, int>> candidatos;

    for (int i = 0; i < g->getOrdem(); i++)
    {
        matriz[i] = new int[g->getOrdem()];
        for (int j = 0; j < g->getOrdem(); j++)
        {
            matriz[i][j] = 0;
        }
    }

    inicializaMatriz(g, matriz);

    int *inicializacaoIndices = new int[num_clusters];

    int *nosInseridos = new int[g->getOrdem()];
    for (int i = 0; i < g->getOrdem(); i++)
    {
        nosInseridos[i] = -1; // Flag -1 indica que ainda não foi inserido
    }

    for (int i = 0; i < num_clusters; i++)
    {
        int idRandom = rand() % g->getOrdem(); // Id de um nó
        while (nosInseridos[idRandom] != -1)
        {
            idRandom = rand() % g->getOrdem();
        }
        inicializacaoIndices[i] = idRandom; // Para cada indice que representa o indice de um cluster, nessa posição estará o valor de qual nó que o inicializará.
        nosInseridos[idRandom] = i;         // Na posição idRandom que é o id de um nó eu taco qual cluster que ele vai entrar
    }

    int k = 0;
    Cluster **melhorSolucao = nullptr;
    ofstream arqAlfas("../alfas.txt");

    int indexAlfa;
    float alfa;
    while (k < num_iteracoes)
    {
        // cout << "Valor de k -> " << k << endl;

        // k != 0 para evitar divisão por 0 na primeira iteração
        if (k % bloco == 0 && k != 0)
        {

            if (escolhasAlfa[indexAlfa] != 0)
            {
                medias[indexAlfa] = somaSolucoes[indexAlfa] / escolhasAlfa[indexAlfa]; // soma das solucoes pelo numero de vezes que o alfa i foi escolhido
                atualizaProbabilidades(P, medias, alfas, melhorSolucaoGeral, m);
            }
        }

        // Inicialização dos num_clusters com nós aleatórios.
        int cont;
        for (cont = 0; cont < num_clusters; cont++)
        {
            clusters[cont]->inserirNoCluster(g->findNoById(inicializacaoIndices[cont]));
            nosInseridos[inicializacaoIndices[cont]] = cont; // Reinicializar de novo
        }

        // Cont vai chegar aqui valendo num_clusters

        candidatos = rankeiaCandidatos(g, clusters, nosInseridos, matriz, num_clusters, min, max);

        indexAlfa = escolheAlfa(P, m);
        alfa = alfas[indexAlfa];

        
        // Loop para encontrar uma solução com alfa e armazenar na variável clusters
        while (cont < g->getOrdem() && !candidatos.empty())
        {
            // cout << "~~~~~~~~~~~~~~~~Candidatos~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
            // for (std::forward_list<std::tuple<int, int, float, int>>::iterator it = candidatos.begin(); it != candidatos.end(); it++)
            // {
            //     cout << "Id nó: " << std::get<0>(*it) << " Id do cluster: " << std::get<1>(*it) << " Contribuição: " << std::get<2>(*it) << endl;
            // }
            // cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

            // cout << "Debug 1\n";

            int index = randomRange(std::distance(candidatos.begin(), candidatos.end()), alfa);
            std::tuple<int, int, float, int> top;
            int contador = 0;

            for (std::forward_list<std::tuple<int, int, float, int>>::iterator it = candidatos.begin(); it != candidatos.end(); it++)
            {
                if (index == contador)
                {
                    top = (*it);
                    break;
                }
                else
                {
                    contador++;
                }
            }

            // cout << "Id do nó que está inserindo: " << std::get<0>(top) << endl;
            clusters[std::get<1>(top)]->inserirNoCluster(g->findNoById(std::get<0>(top)));

            // cout << "Debug 3\n";
            nosInseridos[std::get<0>(top)] = std::get<1>(top);

            // cout << "Debug 4\n";
            candidatos = atualizaCandidatos(g, clusters, nosInseridos, num_clusters, min, max, candidatos, matriz, std::get<0>(top), std::get<1>(top));

            cont += 1;
        }

        if (solucao(clusters, num_clusters))
        {
            float qualidadeSolAtual = calculaQualidadeSolucao(clusters, num_clusters);
            escolhasAlfa[indexAlfa] = escolhasAlfa[indexAlfa] + 1;                 // só incrementa o alfa utilizado, se for encontrada uma solução viável
            somaSolucoes[indexAlfa] = somaSolucoes[indexAlfa] + qualidadeSolAtual; // acumula a soma das soluções para esse alfa escolhido

            if (melhorSolucao == nullptr) // primeira solução
            {
                melhorSolucao = copiaClusters(clusters, num_clusters, g);
                // solBest[indexAlfa].melhorSolucao = qualidadeSolAtual;
                melhorSolucaoGeral.melhorSolucao = qualidadeSolAtual;
            }
            else if (qualidadeSolAtual > calculaQualidadeSolucao(melhorSolucao, num_clusters))
            {
                
                melhorSolucao = copiaClusters(clusters, num_clusters, g);
                // solBest[indexAlfa].melhorSolucao = qualidadeSolAtual;
                melhorSolucaoGeral.melhorSolucao = qualidadeSolAtual;
            }

        }

        // Zera todos os clusters para a próxima iteração.
        for (int i = 0; i < g->getOrdem(); i++)
        {
            if (i < num_clusters)
            {
                clusters[i]->zeraCluster(g);
            }
            nosInseridos[i] = -1; // Resetar
        }

        k++;
    }

    if (melhorSolucao == nullptr)
    {
        cout << "Não encontrou NENHUMA solução..." << endl;
    }else{
        escreveNosCluster(melhorSolucao, num_clusters, pathOut);
    }
}

#endif