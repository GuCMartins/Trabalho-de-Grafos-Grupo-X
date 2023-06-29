#ifndef GREEDY_ALGORITHMS_CPP
#define GREEDY_ALGORITHMS_CPP

#include "../include/Grafo.h"
#include "../include/Cluster.h"
#include <iostream>
#include <forward_list>
#include <queue>
#include <tuple>

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


bool solucao(Cluster** clusters, int num_clusters){
    for(int i = 0; i < num_clusters; i++){
        if(!clusters[i]->getDentroIntervalo())
            return false;
    }
    return true;
}

//Estrutura auxiliar para ajudar a controlar a questão das arestas a serem inseridas inicialmente.
typedef struct{
    int NoOrigem, NoDest;
    float pesoAresta;
} NoNoArco;

bool compareArco(NoNoArco a, NoNoArco b) {
    return a.pesoAresta > b.pesoAresta;
}

std::forward_list<NoNoArco> ordenaGrafoPorPesoAresta(Grafo *g,int max,int **matriz){
    std::forward_list<NoNoArco> OriDestPeso;
    No *noOrigem;
    Arco *arco;
    for(int i = 0; i < g->getOrdem(); i++){
        
        noOrigem = g->findNoById(i);
        
        arco = noOrigem->getAdjacentes();
        
        //Analisar apenas a parte superior triangular superior da matriz para nao inserir arestas repetidas
        while(arco != nullptr){
            if(arco->getNodeDest() > noOrigem->getId()){
                //Caso os dois nós que compoem a aresta estejam dentro do intervalo
                if(noOrigem->getPeso() + g->findNoById(arco->getNodeDest())->getPeso() <= max){
                    NoNoArco obj;
                    obj.NoOrigem = noOrigem->getId();
                    obj.NoDest = arco->getNodeDest();
                    obj.pesoAresta = arco->getPeso();
                    OriDestPeso.push_front(obj); 
                }
                matriz[i][arco->getNodeDest()] = arco->getPeso();
            }
            arco = arco->getProx();
        }
    }

    OriDestPeso.sort(compareArco);
    return OriDestPeso;
}

bool compareCandidatos(std::tuple<int, int, float> a, std::tuple<int, int, float> b){
    return std::get<2>(a) > std::get<2>(b);
}

std::forward_list<std::tuple<int, int, float>> rankeiaCandidatos(Grafo *g, Cluster** clusters, int* nosInseridos, int** matriz, int num_clusters,int min,int max){
    
    std::forward_list<std::tuple<int, int, float>> candidatos;
    float auxContribuicao;
    //Para cada nó que ainda não está em nenhum cluster.
    for(int i = 0; i < g->getOrdem(); i++){
        if(nosInseridos[i] == -1){

            //Para cada cluster avaliado
            for(int j = 0; j < num_clusters; j++){

                //Laço para computar a contribuição do nó em cluster da lista, caso a inserção do nó não viole as restrições do problema.
                
                if(clusters[j]->getSumVertices() + g->findNoById(i)->getPeso() <= max){
                    auxContribuicao = 0;   
                    std::forward_list<int> inseridos = clusters[j]->getInseridos(); 

                    for(std::forward_list<int>::iterator it = inseridos.begin(); it != inseridos.end(); it++){
                        if(i > *it)
                            auxContribuicao += matriz[*it][i];
                        else
                            auxContribuicao += matriz[i][*it];
                    }  

                    

                    //!!!!!!!!!!!!!
                    std::tuple<int, int, float> tup;
                    //* Tupla consiste na trinca: id do nó que ainda não foi inserido em nenhum cluster, id do cluster, contribuição para o cluster.
                    tup = std::make_tuple(i, j, auxContribuicao);
                    candidatos.push_front(tup);
                }

            }

        }
    }

    candidatos.sort(compareCandidatos);
    return candidatos;
}


std::forward_list<std::tuple<int, int, float>> atualizaCandidatos(Grafo *g, Cluster** clusters, int* nosInseridos, int num_clusters,int min,int max, std::forward_list<std::tuple<int, int, float>> candidatos, int **matriz, int ultimoInserido, int idClusterUltimoInserido){
    //Atualizar a lista de candidatos em relação a esse cara que foi o último a ser inserido.
    std::forward_list<std::tuple<int, int, float>> newCandidatos;

    for(std::forward_list<std::tuple<int, int, float>>::iterator it = candidatos.begin(); it != candidatos.end(); it++){
        if(std::get<0>(*it) != ultimoInserido){
            if(std::get<1>(*it) != idClusterUltimoInserido){
                newCandidatos.push_front(*it);
            }else{
               if(clusters[std::get<1>(*it)]->getSumVertices() + g->findNoById(std::get<0>(*it))->getPeso() <= max){
                    float auxContribuicao = 0; 

                    std::get<2>(*it) += matriz[std::get<0>(*it)][ultimoInserido];

                    newCandidatos.push_front(*it);
                } 
            }
        }
    }

    newCandidatos.sort(compareCandidatos);
    return newCandidatos;
}

/*
    '''Dado um grafo g e um vetor com os clusters, essa função será capaz de devolver em um arquivo .txt uma solução aproximada para o problema de clusterização capacitada. Nesse arquivo, estarão contidos a soma dos pesos das arestas de cada cluster, nós que cada cluster possui, e soma total das arestas dos clusters (Qualidade da solução).''' 

    ? g: Grafo que será analisado
    ? param clusters: Vetor de clusters instanciado durante a leitura do arquivo de instâncias do problema
    ? param num_clusters: Inteiro que representa o tamanho do vetor de clusters
    ? param min, max: Inteiros que representam o valor máximo e mínimo do intervalo dos clusters
*/
void guloso(Grafo *g, Cluster** clusters, int num_clusters,int min,int max){
    
    int cont = 0;
    int **matriz = new int*[g->getOrdem()];
    std::tuple<int, int, float> top;
    std::forward_list<std::tuple<int, int, float>> candidatos;

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

    
    // Obtem as arestas viaveis ordenadas por peso  
    std::forward_list<NoNoArco> listaArestaPorPeso = ordenaGrafoPorPesoAresta(g,max,matriz); 
    
    // for(std::forward_list<NoNoArco>::iterator it = listaArestaPorPeso.begin(); it != listaArestaPorPeso.end(); it++){
    //     cout << (*it).NoOrigem << " " << (*it).NoDest << " " << (*it).pesoAresta << endl;
    // }

    /*
    A partir daqui, tem-se:
    listaArestaPorPeso: lista de arestas ordenadas por peso, sendo que peso(i) + peso(j) <= max
    matriz: matriz de adjacencia do grafo g, onde cada posicao [i][j] contem o peso da aresta que liga o no i ao no j
    */
    
    while(cont < g->getOrdem() || !(solucao(clusters, num_clusters))){
        cout << "<<<<<<<cont: " << cont << ">>>>>>>>\n";
        if(cont < 2*num_clusters){
            //Parte da inserção dos 2 * (num_clusters) vértices;
            
            while(nosInseridos[listaArestaPorPeso.front().NoOrigem] != -1 || nosInseridos[listaArestaPorPeso.front().NoDest] != -1) {
                //Se algum dos caras da aresta tem valor diferente de -1, significa que já estão presentes em algum cluster. 
                listaArestaPorPeso.pop_front();
            }
            
            clusters[cont/2]->inserirNoCluster(g->findNoById(listaArestaPorPeso.front().NoOrigem));
            
            nosInseridos[listaArestaPorPeso.front().NoOrigem] = cont;
            
            clusters[cont/2]->inserirNoCluster(g->findNoById(listaArestaPorPeso.front().NoDest));
            nosInseridos[listaArestaPorPeso.front().NoDest] = cont;
            
            listaArestaPorPeso.pop_front();

            cont += 2;
        }else{

            if(cont == 2*num_clusters)
                candidatos = rankeiaCandidatos(g, clusters, nosInseridos, matriz, num_clusters, min, max);

            
            for(std::forward_list<std::tuple<int, int, float>>::iterator it = candidatos.
            begin(); it != candidatos.end(); it++){
                cout << std::get<0>(*it) << " " << std::get<1>(*it) << " " << std::get<2>(*it) << endl;
            }
            cout << "-------------------------><>" << endl;

            top = candidatos.front();
            clusters[std::get<1>(top)]->inserirNoCluster(g->findNoById(std::get<0>(top)));
            nosInseridos[std::get<0>(top)] = std::get<1>(top);
            candidatos = atualizaCandidatos(g, clusters, nosInseridos, num_clusters, min, max, candidatos, matriz, std::get<0>(top), std::get<1>(top));
            cont++;
        }
    }

    float qualidadeSolucao = 0;
    for(int i = 0; i < num_clusters; i++){
        clusters[i]->getGrafo()->imprimirTodosNosAdjacentes();
        qualidadeSolucao += clusters[i]->getSumArestas();
        cout << "---------------------------------------------" << endl;
    }
    cout << "Qualidade desse negócio: " << qualidadeSolucao << endl;
    // cout << "Impressão da Solução" << endl;
    // for(int i = 0; i < num_clusters; i++)
    // {
    //     cout << clusters[i]->getInseridos().max_size() << endl;
    //     while(!clusters[i]->getInseridos().empty()){
    //         cout << clusters[i]->getInseridos().front() << " ";
    //         clusters[i]->getInseridos().pop_front();
    //     }

    //     cout << endl;
    // }
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