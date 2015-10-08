#!/bin/bash
for i in `seq 0 9`;
do
	./lab5 < input/arq0$i.in.txt > my_outputs/$i.out.txt
done
