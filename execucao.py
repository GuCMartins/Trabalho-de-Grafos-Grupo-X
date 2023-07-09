import subprocess
import os

os.chdir('./build')
proc = subprocess.Popen(["cmake",".."])
proc.wait()

proc = subprocess.Popen(["make"])
proc.wait()


num_iteracoes = "500"
bloco = "50"

alfa_randomizado = "0.35"
alfas_randomizado = ["0.05", "0.10", "0.15", "0.30", "0.5"]

# Lista de parâmetros a serem passados para o programa
pasta_instancias = "../instancias/"

"""
parametros = [
    [pasta_instancias+"20_5_270001","../saida.txt","Handover", "0", "1", "0", "G"],
    [pasta_instancias+"20_5_270001","../saida.txt","Handover", "0", "1", "0", "GRA", num_iteracoes, alfa_randomizado],
    [pasta_instancias+"20_5_270001","../saida.txt","Handover", "0", "1", "0", "GRAR", num_iteracoes, bloco],
    
    [pasta_instancias+"20_10_270001","../saida.txt","Handover", "0", "1", "0", "G"],
    [pasta_instancias+"20_10_270001","../saida.txt","Handover", "0", "1", "0", "GRA", num_iteracoes, alfa_randomizado],
    [pasta_instancias+"20_10_270001","../saida.txt","Handover", "0", "1", "0", "GRAR", num_iteracoes, bloco],
    
    [pasta_instancias+"30_5_270003","../saida.txt","Handover", "0", "1", "0", "G"],
    [pasta_instancias+"30_5_270003","../saida.txt","Handover", "0", "1", "0", "GRA", num_iteracoes, alfa_randomizado],
    [pasta_instancias+"30_5_270003","../saida.txt","Handover", "0", "1", "0", "GRAR", num_iteracoes, bloco],
    
    [pasta_instancias+"RanReal240_01.txt","../saida.txt","RanReal240", "0", "1", "0", "G"],
    [pasta_instancias+"RanReal240_01.txt","../saida.txt","RanReal240", "0", "1", "0", "GRA", num_iteracoes, alfa_randomizado],
    [pasta_instancias+"RanReal240_01.txt","../saida.txt","RanReal240", "0", "1", "0", "GRAR", num_iteracoes, bloco],
    
    [pasta_instancias+"RanReal240_04.txt","../saida.txt","RanReal240", "0", "1", "0", "G"],
    [pasta_instancias+"RanReal240_04.txt","../saida.txt","RanReal240", "0", "1", "0", "GRA", num_iteracoes, alfa_randomizado],
    [pasta_instancias+"RanReal240_04.txt","../saida.txt","RanReal240", "0", "1", "0", "GRAR", num_iteracoes, bloco],
    
    [pasta_instancias+"RanReal480_01.txt","../saida.txt","RanReal480", "0", "1", "0", "G"],
    [pasta_instancias+"RanReal480_01.txt","../saida.txt","RanReal480", "0", "1", "0", "GRA", num_iteracoes, alfa_randomizado],
    [pasta_instancias+"RanReal480_01.txt","../saida.txt","RanReal480", "0", "1", "0", "GRAR", num_iteracoes, bloco],
    
    [pasta_instancias+"RanReal480_04.txt","../saida.txt","RanReal480", "0", "1", "0", "G"],
    [pasta_instancias+"RanReal480_04.txt","../saida.txt","RanReal480", "0", "1", "0", "GRA", num_iteracoes, alfa_randomizado],
    [pasta_instancias+"RanReal480_04.txt","../saida.txt","RanReal480", "0", "1", "0", "GRAR", num_iteracoes, bloco],
    
    [pasta_instancias+"RanReal960_01.30.txt","../saida.txt","RanReal960", "0", "1", "0", "G"],
    [pasta_instancias+"RanReal960_01.30.txt","../saida.txt","RanReal960", "0", "1", "0", "GRA", num_iteracoes, alfa_randomizado],
    [pasta_instancias+"RanReal960_01.30.txt","../saida.txt","RanReal960", "0", "1", "0", "GRAR", num_iteracoes, bloco],
    
    [pasta_instancias+"Sparse82_02.txt","../saida.txt","Sparse82", "0", "1", "0", "G"],
    [pasta_instancias+"Sparse82_02.txt","../saida.txt","Sparse82", "0", "1", "0", "GRA", num_iteracoes, alfa_randomizado],
    [pasta_instancias+"Sparse82_02.txt","../saida.txt","Sparse82", "0", "1", "0", "GRAR", num_iteracoes, bloco]
]
"""

# Experimento 1
# Para cada instância:
    # Rodar o algoritmo guloso uma vez. 
    # Anotar o valor da solução e o tempo de processamento para cada execução.

parametros = [
    [pasta_instancias+"20_5_270001","../saida.txt","Handover", "0", "1", "0", "G"],
    
    [pasta_instancias+"20_10_270001","../saida.txt","Handover", "0", "1", "0", "G"],
    
    [pasta_instancias+"30_5_270003","../saida.txt","Handover", "0", "1", "0", "G"],
    
    [pasta_instancias+"RanReal240_01.txt","../saida.txt","RanReal240", "0", "1", "0", "G"],
    
    [pasta_instancias+"RanReal240_04.txt","../saida.txt","RanReal240", "0", "1", "0", "G"],
    
    [pasta_instancias+"RanReal480_01.txt","../saida.txt","RanReal480", "0", "1", "0", "G"],
    
    [pasta_instancias+"RanReal480_04.txt","../saida.txt","RanReal480", "0", "1", "0", "G"],
    
    [pasta_instancias+"RanReal960_01.30.txt","../saida.txt","RanReal960", "0", "1", "0", "G"],
    
    [pasta_instancias+"Sparse82_02.txt","../saida.txt","Sparse82", "0", "1", "0", "G"]
]
    

for parametro in parametros:
    for i in range(10):
        comando = ["./Grupo_1"]+parametro
        print(comando)
        proc = subprocess.Popen(comando)
        proc.wait()
