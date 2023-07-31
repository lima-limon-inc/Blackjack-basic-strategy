CC = gcc
CFLAGS = -g -W -Wall -Werror
#-fanalyzer

OBJECTFILES = cards.o player.o dealer.o blackjackTable.o graphics.o playerHand.o blackjackRules.o basicStrategy.o main.o

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

blackjackTable.o: blackjackTable.c
	$(CC) $(CFLAGS) -c blackjackTable.c

graphics.o: graphics.c
	$(CC) $(CFLAGS) -c graphics.c

blackjackRules.o: blackjackRules.c
	$(CC) $(CFLAGS) -c blackjackRules.c

playerHand.o: playerHand.c
	$(CC) $(CFLAGS) -c playerHand.c

basicStrategy.o: basicStrategy.c
	$(CC) $(CFLAGS) -c basicStrategy.c

build: $(OBJECTFILES)
	clear
	$(CC) $(CFLAGS) -o main.out $(OBJECTFILES)

valgrind: build
	valgrind --leak-check=full ./main.out

gdb: build
	gdb ./main.out
