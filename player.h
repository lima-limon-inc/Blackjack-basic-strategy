#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include <stdbool.h>

#include "playerHand.h"

#define MAXAMOUNTOFSPLITS 4

typedef struct player{
	char name[20];

	//Money!
	int funds;

	int howManyHands;

	playerHand *playerHands[MAXAMOUNTOFSPLITS];

}player;

player *createPlayer(char *playerName, int initialFunds);

void killPlayer(player *playerPtr);

void receiveCard(player *playerPtr, card *newCard, int whichHand);

void removeCards(player *playerPtr, int whichHand);

void printCards(player *playerPtr, int whichHand);

void makeABet(player *playerPtr, int bet, int whichHand);

int loseBet(player *playerPtr, int whichHand);
		
void winBet(player *playerPtr, int awardedMoney, int whichHand);

int getBet(player *playerPtr, int whichHand);

int getNumberOfHands(player *playerPtr);

//TODO: DO generic macro thing with getSpecificHandDealer
playerHand *getSpecificHand(player *playerPtr, int whichHand);

void increaseBet(player *playerPtr, int increaseAmount, int whichHand);

#endif
