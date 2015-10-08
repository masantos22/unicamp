#!/bin/bash
for i in `seq 0 9`;
do
	diff my_outputs/$i.out.txt  output/arq0$i.res.txt > diff/$i.txt
done
