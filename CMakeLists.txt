cmake_minimum_required(VERSION 3.0.0)
project(Trabalho_de_Grafos VERSION 0.1.0)

include(CTest)
enable_testing()
add_library(lib-Trabalho_de_Grafos
            ../include/Arco.h 
            ../object/Arco.cpp
            ../include/Grafo.h
            ../object/Grafo.cpp
            ../include/No.h
            ../object/No.cpp
            ../include/Cluster.h
            ../object/Cluster.cpp
            ../apps/grafos_util.h
            ../apps/greedy_algorithms.h)

add_executable(Grupo_1 apps/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(Grupo_1 PUBLIC lib-Trabalho_de_Grafos)