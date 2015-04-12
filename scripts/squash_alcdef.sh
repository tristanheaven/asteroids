#!/bin/bash

dir=ALCDEF_ALL

dos2unix "${dir}"/*.txt > /dev/null

exec sed -i \
	-e '/^\(ENDDATA\|DATA=\)/!d' \
	-e '/^DATA=/s:DATA=\(.*\)|\(.*\)|\(.*\)|.*:\1 \2:' \
	-e 's:ENDDATA::' \
	"${dir}"/*.txt
