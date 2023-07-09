run: build
	clear
	./main.out

cards.o: cards.c
	gcc -c cards.c

main.o: main.c
	gcc -c main.c

build: cards.o main.o
	gcc -o main.out cards.o main.o
