from __future__ import print_function
import random
import os
   
quote_file = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'quotes.txt')
f = open(quote_file, 'r')
txt = f.read()
lines = txt.split('\n.\n')
f.close()
print (random.choice(lines))