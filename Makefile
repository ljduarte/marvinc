CC 		= gcc
CFLAGS 	= -O3 -I./src
RM		= rm -f
EXEC = /bin/MarvinC
default:	gnuplot_i.o MarvinC

gnuplot_i.o: src/gnuplot_i.c src/gnuplot_i.h 
	$(CC) $(CFLAGS) -c -o gnuplot_i.o src/gnuplot_i.c -w

MarvinC: src/config src/func.h src/gnuplot_i.h
	$(CC) $(CFLAGS) -o $(EXEC) src/marvinc.c gnuplot_i.o -lm -w 

clean:
	$(RM) gnuplot_i.o $(EXEC)

