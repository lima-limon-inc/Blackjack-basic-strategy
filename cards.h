#ifndef CARDS_HEADER
#define CARDS_HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>

typedef enum suits {Clubs, Diamonds, Hearts, Spades} suits;
typedef struct card{
//Nomenclature taken from: https://en.wikipedia.org/wiki/Standard_52-card_deck

	/* short uint rank; */
	int rank;

	//Suit: Clubs ♣, Diamonds ♦, Hearts ♥, Spades ♠
	suits suit;
}card;


#define SUITAMOUNT 4
#define DECKLENGTH 13
#define CARDSINADECK (DECKLENGTH * SUITAMOUNT)
typedef struct deck {
	card cards[CARDSINADECK];

} deck;

#define DECKAMOUNT 8
#define CARDSINASHOE (DECKAMOUNT * CARDSINADECK)
typedef struct shoe {
	uint topOfShoe;

	card *cards[CARDSINASHOE];

} shoe;

typedef struct deckAndShoe {
	//This contains all the pointers to the cards present in the shoe item
	deck *pokerDeck;

	shoe *pokerShoe;

} deckAndShoe;

deckAndShoe* createPokerGame();
void destroyPokerGame(deckAndShoe *oldPokerGame);

#endif
