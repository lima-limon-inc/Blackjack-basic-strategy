#include "pokerTable.h"
#include "player.h"

#define INITIALCAPACITY 5

pokerTable *createPokerTable(int initialFunds) {
	dealer *dealerPtr = createDealer(initialFunds);

	pokerTable *newPokerTable;	
	newPokerTable =  (pokerTable *) malloc(sizeof(pokerTable) +
			INITIALCAPACITY * sizeof(player));

	newPokerTable->pokerDealer = dealerPtr;
	newPokerTable->playerCapacity = INITIALCAPACITY;
	newPokerTable->playerAmount = 0;

	return newPokerTable;
}

/* void addPlayer(pokerTable *pokerTable,player *newPlayer) { */
/* 	pokerTable-> */
	
/* } */
