# Ahnaf Raihan

all:	prog2tree

prog2tree:	main.o main.c
	gcc  main.o -o prog2tree

main.o:	main.c
	gcc -std=c11 -c -w  main.c

clean:
	rm -f *.o prog2tree

