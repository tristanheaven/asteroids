#!/bin/bash

dir="ALCDEF_ALL"

for f in "${dir}"/*.txt ; do
	class=$(scripts/classify.sh "${f}")
	[[ -z ${class} ]] && continue
	./metrics "${f}" "${class}" | tail -n 1
done
