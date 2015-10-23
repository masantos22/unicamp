import os
import re

path = '../Alocacao_Propaganda/backtracking/data/'
files = os.listdir(path)

t = 0
maior = 0
menor = 9999999999
for i in files:
	m = re.search(r'\d_NODES.txt',i)
	if m:
		r = os.popen('wc -l ' + path + i).read()
		r = int(r.split()[0])
		t += r
		if r > maior:
			maior = r
		if r < menor:
			menor = r

print t/20.0
print maior
print menor
