import os
from subprocess import Popen, PIPE

for i in range(5,19,2):
	os.system("./generate_triangulated_graph.e 100 " + str(i) + " 200 200 > arq3.in")
	os.system("./transmissao_dados.e arq3.in >> out.txt")
