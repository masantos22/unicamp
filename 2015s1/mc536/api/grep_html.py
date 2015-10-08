#!/bin/bash/python

import os


output = 'output'
cities = os.listdir(output)

for city in cities:
	htmls = os.listdir(output+'/'+city+'/html')
	#os.makedirs(output+'/'+city+'/grep')
	for html in htmls:
		f = output+'/'+city+'/html/'+html
		out = output+'/'+city+'/grep/'+html
		os.system('cat '+f+' | grep -E "<td" > '+out)