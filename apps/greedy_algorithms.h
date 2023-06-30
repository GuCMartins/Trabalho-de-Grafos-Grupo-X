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

std::forward_list<NoUtils> ordenaGrafoPorGrauNo(Grafo *g,float max,int **matriz){
    std::forward_list<NoUtils> idGrau;
    No *noOrigem;
    Arco *arco;
    for(int i = 0; i < g->getOrdem(); i++){
        
        noOrigem = g->findNoById(i);
        
        arco = noOrigem->getAdjacentes();

        // Caso o nó que está sendo avaliado esteja dentro do intervalo considerando o cluster inicialmente vazio. 
        if (noOrigem->getPeso() <= max)
        {
            NoUtils obj;
            obj.idNo = noOrigem->getId();
            obj.grau = noOrigem->getGrauEntrada();
            obj.peso = noOrigem->getPeso();
            idGrau.push_front(obj);
        }

        //Analisar apenas a parte superior triangular superior da matriz para nao inserir arestas repetidas -> SÓ PARA A MATRIZ !!!
        while(arco != nullptr){
            if(arco->getNodeDest() > noOrigem->getId()){
                matriz[i][arco->getNodeDest()] = arco->getPeso();
            }
            arco = arco->getProx();
        }
    }

    idGrau.sort(compareNos);
    return idGrau;
}

bool compareCandidatos(std::tuple<int, int, float, int> a, std::tuple<int, int, float, int> b){
    return std::get<2>(a) > std::get<2>(b);
}

std::forward_list<std::tuple<int, int, float, int>> rankeiaCandidatos(Grafo *g, Cluster** clusters, int* nosInseridos, int** matriz, int num_clusters,float min,float max){
    
    std::forward_list<std::tuple<int, int, float, int>> candidatos;
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
                    std::tuple<int, int, float, int> tup;
                    //* Tupla consiste na trinca: id do nó que ainda não foi inserido em nenhum cluster, id do cluster, contribuição para o cluster.
                    tup = std::make_tuple(i, j, auxContribuicao, g->findNoById(i)->getPeso());
                    candidatos.push_front(tup);
                }

            }

        }
    }

    candidatos.sort(compareCandidatos);
    return candidatos;
}


std::forward_list<std::tuple<int, int, float, int>> atualizaCandidatos(Grafo *g, Cluster** clusters, int* nosInseridos, int num_clusters,float min,float max, std::forward_list<std::tuple<int, int, float, int>> candidatos, int **matriz, int ultimoInserido, int idClusterUltimoInserido){
    //Atualizar a lista de candidatos em relação a esse cara que foi o último a ser inserido.
    std::forward_list<std::tuple<int, int, float, int>> newCandidatos;

    for(std::forward_list<std::tuple<int, int, float, int>>::iterator it = candidatos.begin(); it != candidatos.end(); it++){
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
    
    int cont = 0;
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

    cout << "Comecemos por aqui \n";
    // Obtem os vértices ordenados pelo grau.   
    std::forward_list<NoUtils> listaNosPorGrau = ordenaGrafoPorGrauNo(g,max,matriz);
    for(std::forward_list<NoUtils>::iterator it = listaNosPorGrau.begin(); it != listaNosPorGrau.end(); it++){
        cout << "Id do no: " << (*it).idNo << " Peso do no: " << (*it).peso << endl;
    }
    cout << "Passou da ordenaGrafoPorGrauNo \n";
    
    /*
    A partir daqui, tem-se:
    listaNosPorGrau: lista de nós ordenadas por grau, peso(i) <= max
    matriz: matriz de adjacencia do grafo g, onde cada posicao [i][j] contem o peso da aresta que liga o no i ao no j
    */
   
    
    while(cont < g->getOrdem()){
        
        if(cont < num_clusters){
            //Parte da inserção dos (num_clusters) vértices;
            
            clusters[cont]->inserirNoCluster(g->findNoById(listaNosPorGrau.front().idNo));
            nosInseridos[listaNosPorGrau.front().idNo] = cont;
            
            listaNosPorGrau.pop_front();
            
        }else{
            cout << "------- Processo de debug : Cont -> " << cont << " ----- " << endl;

            if(cont == num_clusters)
                candidatos = rankeiaCandidatos(g, clusters, nosInseridos, matriz, num_clusters, min, max);

            cout << "/********************Normal************************/" << endl;
            
            for(std::forward_list<std::tuple<int, int, float, int>>::iterator it = candidatos.begin(); it != candidatos.end(); it++){
                cout << "Id do no candidato: " << std::get<0>(*it) << " Id do cluster: " << std::get<1>(*it) << " Contribuicao: " << std::get<2>(*it) << " Peso do no: " << std::get<3>(*it) << endl;
            }    

            cout << "/********************************************************/" << endl;

            std::forward_list<std::tuple<int, int, float, int>> candidatosEmpate;
            std::tuple<int, int, float, int> anterior;
            bool flagAux = true;
            for(std::forward_list<std::tuple<int, int, float, int>>::iterator it = candidatos.begin(); it != candidatos.end(); it++){
                if(flagAux){
                    anterior = *it;
                    flagAux = false;
                }else{
                    if(std::get<2>(anterior) == std::get<2>(*it)){
                        anterior = *it;
                    }else{
                        break;
                    } 
                }
                candidatosEmpate.push_front(anterior);
            }    
            candidatosEmpate.sort(comparePesos);

            cout << "/*****************Empate****************************/" << endl;
            
            for(std::forward_list<std::tuple<int, int, float, int>>::iterator it = candidatosEmpate.begin(); it != candidatosEmpate.end(); it++){
                cout << "Id do no candidato: " << std::get<0>(*it) << " Id do cluster: " << std::get<1>(*it) << " Contribuicao: " << std::get<2>(*it) << " Peso do no: " << std::get<3>(*it) << " Soma dos vertices naquele cluster que ele pode ir: " << clusters[std::get<1>(*it)]->getSumVertices() << endl;
            }    

            cout << "/********************************************************/" << endl;

            bool flagAux2 = true;
            int menor;
            for(std::forward_list<std::tuple<int, int, float, int>>::iterator it = candidatosEmpate.begin(); it != candidatosEmpate.end(); it++){
                if(flagAux2){
                    //Anterior na verdade é o primeiro
                    anterior = (*it);
                    menor = std::get<1>(*it);
                    flagAux2 = false;
                }else{
                    if(std::get<0>(*it) == std::get<0>(anterior)){
                        if(clusters[std::get<1>(*it)]->getSumVertices() < clusters[menor]->getSumVertices()){
                            menor = std::get<1>(*it);
                        }
                    }
                }
            }

            cout << " @@@ Menor que foi avaliado:  " << menor << endl;

            cout << "Causa de ERRO 1\n";
            top = candidatosEmpate.front();
            cout << "Causa de ERRO 2\n";
            clusters[menor]->inserirNoCluster(g->findNoById(std::get<0>(top)));
            cout << "Causa de ERRO 3\n";
            nosInseridos[std::get<0>(top)] = menor;
            cout << "Causa de ERRO 4\n";
            candidatos = atualizaCandidatos(g, clusters, nosInseridos, num_clusters, min, max, candidatos, matriz, std::get<0>(top), menor);
            
        }

        cont += 1;
    }

    float qualidadeSolucao = 0;
    for(int i = 0; i < num_clusters; i++){
        clusters[i]->getGrafo()->imprimirTodosNosAdjacentes();
        qualidadeSolucao += clusters[i]->getSumArestas();
        cout << "---------------------------------------------" << endl;
    }
    cout << "Qualidade desse troço das guloseima: " << qualidadeSolucao << endl;
    cout << boolalpha << solucao(clusters, num_clusters) << endl;
    for(int i = 0; i < num_clusters; i++){
        cout << "Valor da soma de vertices: " << clusters[i]->getSumVertices() << endl;
    }
    
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