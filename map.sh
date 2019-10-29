#!/bin/bash

FILE=$1
MIN_ID=1
MAX_ID=999

function findCorrectLine() {
	local searchId=$1;
	local correctLine="";
	
	while IFS='' read -r line || [[ -n "$line" ]];
	do
		local tempId=${line%%;*};
		if [ $searchId == $tempId ]; then
			correctLine=$line;
		fi;
	done <$FILE

	echo $correctLine;
}

function isRouteIdCorrect() {
	local routeId=$1;

	if [[ ! $routeId =~ ^[0-9]+$ ]]; then
		exit 1;
	else
		if [ $routeId -lt $MIN_ID ] || [ $routeId -gt $MAX_ID ]; then
			exit 1;
		fi;
	fi;
}

function countRouteLength() {
	local counter=1;
	local description=$1;
	local length=0;

	IFS=';' read -r -a array <<< $description;
	
	for element in ${array[@]};
	do
		if [ $(($counter % 3)) -eq 0 ]; then
			length=$((length + $element));
		fi;

		counter=$((counter + 1));
	done

	IFS=''
	echo $length;
}

if [ $# -lt 2 ] || [ ! -f $FILE ]; then
	exit 1;
fi;

shift; #Pominięcie pierwszego parametru, czyli ścieżki do pliku.

for routeId in "$@"
do
	isRouteIdCorrect $routeId
	correctLine="$(findCorrectLine $routeId)";
	if [ "$correctLine" != "" ]; then
		length="$(countRouteLength $correctLine)";
		echo -e $routeId "\b;" "\b$length";
	fi;
done
