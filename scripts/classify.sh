#!/bin/bash

number=$(basename ${1} | cut -d_ -f2)
class=$(grep "^${number} " mp_properties_number_taxclass.txt | cut -d' ' -f2)

if [[ ${class} == "S" ]] ; then
	echo "1 -1 -1"
elif [[ ${class} == "C" ]] ; then
	echo "-1 1 -1"
elif [[ ${class} == "M" ]] ; then
	echo "-1 -1 1"
else
	echo ""
fi
