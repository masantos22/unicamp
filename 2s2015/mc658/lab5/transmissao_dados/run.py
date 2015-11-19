import os

for i in ["25","50","75","100"]:
	for j in ["1","2","3","4","5","6","7","8","9","10"]:
		#os.system("./generate_triangulated_graph.e "+i+" "+j+" 500 500 > testes/"+i+"_"+j+".in")
		os.system("./transmissao_dados.e testes/"+i+"_"+j+".in > my_r/"+i+"_"+j+".in")