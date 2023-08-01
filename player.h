#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include <stdbool.h>

#include "playerHand.h"

#define MAXAMOUNTOFSPLITS 4

typedef struct player{
	char name[20];

	//Money!
	int funds;

	//Array that contains the bet corresponding to each hand
	int bets[MAXAMOUNTOFSPLITS];

	int howManyHands;

	int insuranceBet;

	playerHand *playerHands[MAXAMOUNTOFSPLITS];

}player;

player *createPlayer(char *playerName, int initialFunds);

void killPlayer(player *playerPtr);

void receiveCard(player *playerPtr, card *newCard, int whichHand);

void printCards(player *playerPtr, int whichHand);

void makeABet(player *playerPtr, int bet, int whichHand);

bool canMakeABet(player *playerPtr, int bet);

int loseBet(player *playerPtr, int whichHand);
		
void winBet(player *playerPtr, int awardedMoney, int whichHand);

int getBet(player *playerPtr, int whichHand);

int getNumberOfHands(player *playerPtr);

//TODO: DO generic macro thing with getSpecificHandDealer
playerHand *getSpecificHand(player *playerPtr, int whichHand);

void increaseBet(player *playerPtr, int increaseAmount, int whichHand);

void splitCards(player *playerPtr, int whichHand);

int getFunds(player *playerPtr);

void resetPlayer(player *playerPtr);

void askForInsurance(player *playerPtr);

#endif
