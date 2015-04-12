#!/bin/bash

dir="ALCDEF_ALL"

for f in "${dir}"/*.txt ; do
	./metrics "${f}" "$(scripts/classify.sh ${f})" | tail -n 1
done
