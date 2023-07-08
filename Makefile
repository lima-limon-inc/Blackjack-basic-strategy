run: build
	./cards.out

cards.o: cards.c
	gcc -c cards.c

build: cards.o
	gcc -o cards.out cards.o
