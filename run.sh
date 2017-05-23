#!/bin/bash

DIR_NAME="result_n2"
mkdir $DIR_NAME

for amp in {1..15}
do
	python gen_square.py $amp
	build/ARM/gem5.opt configs/example/dynamic_freq.py
	mv m5out/stats.txt ${DIR_NAME}/${amp}.txt
done
