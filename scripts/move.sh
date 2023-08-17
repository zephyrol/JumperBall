#!/bin/bash
mv ../maps/map"$1"Uncompress.txt ../maps/intermediate.txt
if [[ $1 -lt $2 ]]
then
	endloop=$(($2-1))
	for i in `seq $1 $endloop`
	do
		next=$(($i+1))
		mv ../maps/map"$next"Uncompress.txt ../maps/map"$i"Uncompress.txt
	done
else
	count=$(($1-$2))
	for i in `seq 1 $count`
	do
		previous=$(($1-$i))
		next=$(($previous+1))
		mv ../maps/map"$previous"Uncompress.txt ../maps/map"$next"Uncompress.txt
	done
fi
mv ../maps/intermediate.txt ../maps/map"$2"Uncompress.txt
./everyone.sh $3
