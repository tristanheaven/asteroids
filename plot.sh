#!/bin/bash

exec gnuplot \
	-e "set terminal svg enhanced background rgb 'white' size 800,600" \
	-e "set output 'plot.svg'" \
	-e "plot '-' using 1 title 'data' with lines" \
	< /dev/stdin
