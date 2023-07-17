CC = gcc
CFLAGS = -g -W -Wall -Werror
#-fanalyzer

OBJECTFILES = cards.o player.o dealer.o pokerTable.o main.o

run: build
	./main.out

cards.o: cards.c
	$(CC) $(CFLAGS) -c cards.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

player.o: player.c
	$(CC) $(CFLAGS) -c player.c

dealer.o: dealer.c
	$(CC) $(CFLAGS) -c dealer.c

pokerTable.o: pokerTable.c
	$(CC) $(CFLAGS) -c pokerTable.c

build: $(OBJECTFILES)
	clear
	$(CC) $(CFLAGS) -o main.out $(OBJECTFILES)
