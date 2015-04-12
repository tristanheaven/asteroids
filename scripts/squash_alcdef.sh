#!/bin/bash

dir=ALCDEF_ALL

dos2unix -q "${dir}"/*.txt

exec sed -i \
	-e '/^\(ENDDATA\|DATA=\)/!d' \
	-e '/^DATA=/s:DATA=\(.*\)|\(.*\)|\(.*\)|.*:\1 \2:' \
	-e 's:ENDDATA::' \
	"${dir}"/*.txt
