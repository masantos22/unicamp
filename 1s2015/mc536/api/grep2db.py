#!/bin/bash/python
import os 

output = 'output'
cities = os.listdir(output)

for city in cities:
	htmls = os.listdir(output+'/'+city+'/grep')
	for html in htmls:
		file_path = output+'/'+city+'/grep/'+html
		out = output+'/'+city+'/db'
		f = open(file_path,'r')
		line_number = 0
		db = ""
		for line in f:
			line_number += 1
			if line_number < 3:
				db += line.split('&')[0].split('>')[1]+','
			elif line_number < 5:
				db += line.split('>')[1].split(' ')[0]
				if line_number != 4:
					db += ','
			elif line_number == 7:
				db += '\n'
				line_number = 0
		f.close()
		f = open(out+'/'+html,'w')
		f.write(db)
		f.close()
		
