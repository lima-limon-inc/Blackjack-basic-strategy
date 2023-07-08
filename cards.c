#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define CARDLENGTH 2
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
#define DECKAMOUNT 8
typedef struct deck {
	card cards[DECKAMOUNT * DECKLENGTH * SUITAMOUNT];

}deck;

deck* createDeckOfCards()
{
	deck* deckOfCards = malloc(sizeof(deck));

	for ( int suitType = Clubs; suitType <= Spades; suitType++) {

		for (int rankNumer = 0 + 13 * suitType; rankNumer <= DECKLENGTH + 13 * suitType; rankNumer++) {
			deckOfCards->cards[rankNumer].rank = rankNumer;
			deckOfCards->cards[rankNumer].suit = suitType;
		}
	}
	
	return deckOfCards;
}

void main() {
	deck* deckOfCards;

	deckOfCards = createDeckOfCards();
	printf("%d", deckOfCards->cards[4].rank);
}
