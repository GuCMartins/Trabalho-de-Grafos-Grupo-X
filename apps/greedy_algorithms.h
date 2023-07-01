#ifndef GREEDY_ALGORITHMS_CPP
#define GREEDY_ALGORITHMS_CPP

#include "../include/Grafo.h"
#include "../include/Cluster.h"
#include <iostream>
#include <forward_list>
#include <queue>
#include <tuple>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <sstream>

using namespace std;

//Algoritmo Guloso

/*

Pseudocódigo para nos auxiliar a fazer o código!

C: Conjunto de candidatos
S: Conjunto que irá conter a solução

função AlgoritmoGuloso(C: conjunto)
    S <- Vazio

    enquanto C != Vazio e não solução(S) faça
        x <- seleciona C
        C <- C{x}
        se é viável S União {x} então
            S <- S União {x}
        fim se
    fim enquanto

    se solução(S) então
        retorne S
    senão
        retorne "Não existe solução!"
    fim se
fim função

*/

//Estrutura auxiliar para ajudar a controlar a questão dos graus dos nós a serem inseridos inicialmente nos clusters.
typedef struct{
    int idNo;
    int grau;
    float peso;
} NoUtils;

bool solucao(Cluster** clusters, int num_clusters){
    for(int i = 0; i < num_clusters; i++){
        if(!clusters[i]->getDentroIntervalo())
            return false;
    }
    return true;
}


bool compareNos(NoUtils a, NoUtils b) {
    return a.peso <= b.peso;
}

void inicializaMatriz(Grafo *g, float max, int **matriz){
    No *noOrigem;
    Arco *arco;
    for(int i = 0; i < g->getOrdem(); i++){
        
        noOrigem = g->findNoById(i);
        
        arco = noOrigem->getAdjacentes();

        while(arco != nullptr){
            
            matriz[i][arco->getNodeDest()] = arco->getPeso();
            
            arco = arco->getProx();
        }
    }
}

//Compara as contribuições dos candidatos.
bool compareCandidatos(std::tuple<int, int, float, int> a, std::tuple<int, int, float, int> b){
    return std::get<2>(a) > std::get<2>(b);
}

std::forward_list<std::tuple<int, int, float, int>> rankeiaCandidatos(Grafo *g, Cluster** clusters, int* nosInseridos, int** matriz, int num_clusters,float min,float max){
    
    std::forward_list<std::tuple<int, int, float, int>> candidatos;
    float auxContribuicao;

    //Para cada nó
    for(int i = 0; i < g->getOrdem(); i++){

        //Se não estiver em algum cluster
        if(nosInseridos[i] == -1){

            //Para cada cluster
            for(int j = 0; j < num_clusters; j++){

                //Laço para computar a contribuição do nó em cluster da lista, caso a inserção do nó não viole as restrições do problema.
                
                //Avaliação de Restrição
                No *analisado = g->findNoById(i); //-> Nó que está sendo analisado no momento
                if(clusters[j]->getSumVertices() + analisado->getPeso() < min){
                    auxContribuicao = 0;   
                    std::forward_list<int> inseridos = clusters[j]->getInseridos(); 

                    for(std::forward_list<int>::iterator it = inseridos.begin(); it != inseridos.end(); it++){
                        auxContribuicao += matriz[*it][i];
                    }  

                    //* Tupla consiste na quadrupla: id do nó que ainda não foi inserido em nenhum cluster, id do cluster, contribuição para o cluster, peso do nó.
                    std::tuple<int, int, float, int> tup;
                    
                    tup = std::make_tuple(i, j, auxContribuicao, analisado->getPeso());
                    candidatos.push_front(tup);
                }else{
                    if(clusters[j]->getSumVertices() + analisado->getPeso() <= max){
                        
                        auxContribuicao = 0;
                        std::forward_list<int> inseridos = clusters[j]->getInseridos();

                        for (std::forward_list<int>::iterator it = inseridos.begin(); it != inseridos.end(); it++)
                        {
                            auxContribuicao += matriz[*it][i];
                        }

                        float residuo = max - clusters[j]->getSumVertices() - analisado->getPeso();
                        float criterioAux = (auxContribuicao * residuo) / analisado->getPeso();

                        //* Tupla consiste na quadrupla: id do nó que ainda não foi inserido em nenhum cluster, id do cluster, contribuição para o cluster, peso do nó.
                        std::tuple<int, int, float, int> tup;
                        
                        tup = std::make_tuple(i, j, criterioAux, analisado->getPeso());
                        candidatos.push_front(tup);
                    }
                }

            }

        }
    }

    candidatos.sort(compareCandidatos);
    return candidatos;
}


std::forward_list<std::tuple<int, int, float, int>> atualizaCandidatos(Grafo *g, Cluster** clusters, int* nosInseridos, int num_clusters, float min, float max, std::forward_list<std::tuple<int, int, float, int>> candidatos, int **matriz, int ultimoInserido, int idClusterUltimoInserido){

    //Atualizar a lista de candidatos em relação a esse cara que foi o último a ser inserido.
    std::forward_list<std::tuple<int, int, float, int>> newCandidatos;

    for(std::forward_list<std::tuple<int, int, float, int>>::iterator it = candidatos.begin(); it != candidatos.end(); it++){

        //último a ser inserido não pode continuar na lista de candidatos.
        if(std::get<0>(*it) != ultimoInserido){
            
            No *analisado = g->findNoById(std::get<0>(*it)); 

            if(std::get<1>(*it) != idClusterUltimoInserido){
                //Esse nó pode continuar pois não alterou nada pra ele a última inserção.
                newCandidatos.push_front(*it);
            }else{
                //Tem relação com o último cluster que foi inserido.

               if(clusters[std::get<1>(*it)]->getSumVertices() + analisado->getPeso() < min){ 

                    std::get<2>(*it) += matriz[std::get<0>(*it)][ultimoInserido];

                    newCandidatos.push_front(*it);
                }else if(clusters[std::get<1>(*it)]->getSumVertices() + analisado->getPeso() <= max){
                    std::get<2>(*it) += matriz[std::get<0>(*it)][ultimoInserido]; //Atualiza a contribuição contabilizando a aresta entre esse avaliddo e o cara que foi inserido.

                    float residuo = max - clusters[std::get<1>(*it)]->getSumVertices() - analisado->getPeso();

                    std::get<2>(*it) = (std::get<2>(*it) * residuo) /  analisado->getPeso();

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
void guloso(Grafo *g, Cluster** clusters, int num_clusters,float min,float max){

    srand(time(NULL));

    int **matriz = new int*[g->getOrdem()];

    std::tuple<int, int, float, int> top;

    std::forward_list<std::tuple<int, int, float, int>> candidatos;

    // Matriz para saber o peso da aresta entre os nós i e j
    for(int i = 0; i < g->getOrdem(); i++){
        matriz[i] = new int[g->getOrdem()];
        for(int j = 0; j < g->getOrdem(); j++){
            matriz[i][j] = 0;
        }
    }

    int *nosInseridos = new int[g->getOrdem()];
    for(int i = 0; i < g->getOrdem(); i++){
        nosInseridos[i] = -1; //Flag -1 indica que ainda não foi inserido
    }  

    inicializaMatriz(g, max, matriz);
   
   //Inicialização dos num_clusters com nós aleatórios.
    int cont;
    for(cont = 0; cont < num_clusters; cont++){
            int idRandom = rand() % g->getOrdem();
            while(nosInseridos[idRandom] != -1){
                idRandom = rand() % g->getOrdem();
            }
            clusters[cont]->inserirNoCluster(g->findNoById(idRandom));
            nosInseridos[idRandom] = cont;
    }

    float qualidadeSolucao = 0;

    ofstream arq("../saidaGuloso2.txt");
    if(arq.is_open()){
        // Cont vai chegar aqui valendo num_clusters

        candidatos = rankeiaCandidatos(g, clusters, nosInseridos, matriz, num_clusters, min, max);

        while (cont < g->getOrdem() && !candidatos.empty())
        {

            arq << "~~~~~~~~~~~~~~~~Candidatos~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
            for(std::forward_list<std::tuple<int, int, float, int>>::iterator it = candidatos.begin(); it != candidatos.end(); it++){
                arq << "Id nó: " << std::get<0>(*it) << " Id do cluster: " << std::get<1>(*it) << " Contribuição: " << std::get<2>(*it) << endl;
            }
            arq << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

            // cout << "Debug 1\n";
            top = candidatos.front();

            // cout << "Debug 2\n";
            clusters[std::get<1>(top)]->inserirNoCluster(g->findNoById(std::get<0>(top)));

            // cout << "Debug 3\n";
            nosInseridos[std::get<0>(top)] = std::get<1>(top);

            // cout << "Debug 4\n";
            candidatos = atualizaCandidatos(g, clusters, nosInseridos, num_clusters, min, max, candidatos, matriz, std::get<0>(top), std::get<1>(top));

            cont += 1;
        }

        for(int i = 0; i < num_clusters; i++){
        arq << "-------------Cluster Analisado: " << i << "--------------------" << endl;

        No *aux = clusters[i]->getGrafo()->getNoInicial();

        if (aux != NULL)
        {
                while (aux != NULL)
                {
                    Arco *arcoAux = aux->getAdjacentes();
                    arq << "Nós adjacentes ao nó: " << aux->getId() << endl;
                    while (arcoAux != NULL)
                    {
                        arq << "NO DESTINO ID: " << arcoAux->getNodeDest() << " PESO DA ARESTA: " << arcoAux->getPeso() << endl;
                        arcoAux = arcoAux->getProx();
                    }
                    aux = aux->getProx();
                }
        }

        qualidadeSolucao += clusters[i]->getSumArestas();

        }

        arq << "É uma solução ? " << boolalpha << solucao(clusters, num_clusters) << endl;
        arq << "Qualidade da nossa solução: " << qualidadeSolucao << endl;
        for(int i = 0; i < num_clusters; i++){
            arq << "Valor da soma de vertices-> " << clusters[i]->getSumVertices() << " para o cluster de id ->" << i << endl;
        }
    }
    arq.close();
    
}



//Algoritmo Guloso Randomizado
void gulosoRandomizado(){
// int nosNaoInseridos = 0;
// for(int i = 0; i < g->getOrdem(); i++){
//     if(nosInseridos[i] == -1){
//         nosNaoInseridos++;
//     }
// }
// float **matrizDecisao = new float*[nosNaoInseridos];
// for(int i = 0; i < nosNaoInseridos; i++){
//     matrizDecisao[i] = new float[num_clusters];
//     for(int j = 0; j < num_clusters; j++){
//         matrizDecisao[i][j] = 0;
//     }
// }
}



//Algoritmo Guloso Randomizado Reativo
void gulosoRandomizadoReativo(){

}



#endif