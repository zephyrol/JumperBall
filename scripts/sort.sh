numbers=( 1 2 3 4 5 7 6 8 10 9 11 15 13 14 12 17 18 26 25 19 22 20 37 56 21 16 23 24 34 27 28 29 51 52 30 50 31 33 36 32 35 38 39 40 59 41 45 42 43 47 48 49 55 46 53 54 87 44 61 84 62 63 58 120 64 65 66 69 68 57 80 75 74 78 70 71 81 72 67 88 82 89 93 94 97 73 83 90 99 91 98 92 95 85 100 101 107 105 103 102 104 60 76 77 113 108 111 117 110 114 79 116 119 86 96 118 106 112 109 115 )

index=1

for num in "${numbers[@]}"
do
	echo "Map $num to Map $index"
	cp ../maps/map"$num"Uncompress.txt ../maps/map"$index"UncompressNew.txt
	((index++))
done

for i in `seq 1 120`
do
	echo "Map $i moving"
	mv ../maps/map"$i"UncompressNew.txt ../maps/map"$i"Uncompress.txt
	echo "Creating compressed Map $i"
	bin/JumperBallApplication ../maps/map"$i"Uncompress.txt -compress
	mv outMapNew.txt ../maps/map"$i".txt
done

make install
