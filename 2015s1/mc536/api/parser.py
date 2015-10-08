#!/bin/bash/python
import urllib2
from dateutil import rrule
import datetime
import os

now = datetime.datetime.now()
past = datetime.date(2013,1,1)

web = "http://www.accuweather.com/pt/br/city/id/january-weather/id?monyr=DATE&view=table"
output = 'output'

f = open('cidades.txt', 'r')
#f = open('test.txt','r')
for line in f:
	city,city_id = line.split(',')[0],line.split(',')[1][:-1]
	custom_web = web.replace('id',str(city_id))
	print city + ' ' + city_id
	if not os.path.exists(output+'/'+city):
		os.makedirs(output+'/'+city)
		os.makedirs(output+'/'+city+'/html')
		os.makedirs(output+'/'+city+'/db')
	for dt in rrule.rrule(rrule.MONTHLY, dtstart=past, until=now):
		date2 = dt.strftime("%m/%d/%Y")
		url = custom_web.replace('DATE',date2)
		print url
		f1 = open(output+'/'+city+'/html/'+city+'_'+dt.strftime("%m-%d-%Y")+'.html','w')
		f1.write(urllib2.urlopen(url).read())
		f1.close()

f.close()
#root = "http://www.accuweather.com/pt/br/city/id/january-weather/id?monyr=DATE&view=table"

#print urllib2.urlopen(web).read()