#!/bin/bash
for i in `seq 1 $1`
do
	echo "Map $i"
	build/build/Debug/GravityGlobe maps/map"$i"Uncompress.txt -compress
	mv outMapNew.txt maps/map"$i".txt
done
ninja install
