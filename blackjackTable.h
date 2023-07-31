#ifndef POKERTABLE_HEADER
#define POKERTABLE_HEADER

#include "dealer.h"
#include "player.h"

typedef struct blackjackTable {
//Nomenclature taken from: https://en.wikipedia.org/wiki/Standard_52-card_deck

	dealer *blackjackDealer;

	int playerCapacity;

	int playerAmount;

	player *players[];
}blackjackTable ;

blackjackTable *createBlackjackTable(int initialFunds);

void destroyBlackjackTable(blackjackTable *blackjackTablePtr);

blackjackTable *addPlayer(blackjackTable *blackjackTablePtr, char name[], int initialFunds);

void blackjackRound(blackjackTable *blackjackTablePtr);

#endif
