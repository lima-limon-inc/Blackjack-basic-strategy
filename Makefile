CC = gcc
CFLAGS = -W -Wall -Werror

run: build
	clear
	./main.out

cards.o: cards.c
	$(CC) $(CFLAGS) -c cards.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

build: cards.o main.o
	$(CC) $(CFLAGS) -o main.out cards.o main.o
