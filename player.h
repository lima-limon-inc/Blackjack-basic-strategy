#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include <stdbool.h>

#define MAXAMOUNTOFSPLITS 4

#include "cards.h"

typedef struct playerHand{

	int cardsInHand;

	int cardCapacity;

	int bet;

	int cardSum;

	card *hand[];

}playerHand;

typedef struct player{
//Nomenclature taken from: https://en.wikipedia.org/wiki/Standard_52-card_deck

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

playerHand *getSpecificHand(player *playerPtr, int whichHand);

void increaseBet(player *playerPtr, int increaseAmount, int whichHand);

void saveCardSum(playerHand *playersHand, int newSum);

/* PLAYERHAND FUNCTIONS */
 
card **getCards(playerHand *playersHand);

int getAmountOfCardsInHand(playerHand *playersHand);

int getHandSum(playerHand *playersHand);

#endif
