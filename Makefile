CC = gcc
CFLAGS = -W -Wall -Werror

run: build
	./main.out

cards.o: cards.c
	$(CC) $(CFLAGS) -c cards.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

cardArithmetic.o: cardArithmetic.c
	$(CC) $(CFLAGS) -c cardArithmetic.c

build: cards.o cardArithmetic.o main.o 
	clear
	$(CC) $(CFLAGS) -o main.out cards.o cardArithmetic.o main.o
