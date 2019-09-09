all:
	gcc -Wall -g -std=c99 -Werror -pthread gen.c
	./a.out
	gcc -Wall -g -std=c99 -Werror -pthread ass1b.c -lm
	./a.out
	gnuplot -e "set terminal jpeg; set xlabel 'No of threads'; set ylabel 'Time'; set title 'Performance comparison'; set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 pi -1 ps 1.5; set pointintervalbox 3; plot 'data2.txt' with linespoints ls 1" > out.jpeg
	eog out.jpeg

