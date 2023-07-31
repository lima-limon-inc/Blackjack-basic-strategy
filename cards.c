#include "cards.h"
#include "stdbool.h"
#include <stdarg.h>
#include <stdlib.h>

static deck *createDeckOfCards()
{
	deck *deckOfCards;
	deckOfCards = (deck *) malloc(sizeof(deck)); 

	for ( suits suitType = Clubs; suitType <= Spades; suitType++) {

		uint offset = DECKLENGTH * suitType;
		for (int rankNumer = 0; rankNumer < DECKLENGTH ; rankNumer++) {
			uint posInArray = rankNumer + offset;

			card newCard = { .rank = rankNumer + 1, .suit = suitType };

			deckOfCards->cards[posInArray] = newCard;
		}
	}

	return deckOfCards;
}

static shoe *createShoeFromDecks(deck *deckOfCards)
{
	shoe *shoeOfCards;
	shoeOfCards = (shoe *) malloc(sizeof(shoe));
	for (int nShoe =0; nShoe < DECKAMOUNT; nShoe++) {

		uint offset = CARDSINADECK * nShoe;
		for (int rankNumer = 0; rankNumer < CARDSINADECK; rankNumer++) {
			uint posInArray = rankNumer + offset;

			//We add a pointer to the card in the deck to the shoe
			card *cardptr = &deckOfCards->cards[rankNumer];
			shoeOfCards->cards[posInArray] = cardptr;
		}
	}

	return shoeOfCards;
}

//Fisher-Yates shuffle:
//https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
static void cardShuffle(card *cards[], uint amountOfCards)
{
	srand(time(NULL));   // Initialization, should only be called once.
	int rankNumer, randomPosition;
	card *tmp;
	for (rankNumer = amountOfCards - 1; rankNumer > 0; rankNumer--) {
		randomPosition = rand() % (rankNumer + 1);
		tmp = cards[randomPosition];
		cards[randomPosition] = cards[rankNumer];
		cards[rankNumer] = tmp;
	}
}

shoe *createShuffledShoe()
{
	deck *deckOfCards = createDeckOfCards();

	shoe *shoeOfCards = createShoeFromDecks(deckOfCards);

	/* for (int i = 0; i < CARDSINASHOE; i++ ) { */
	/* 	printf("%d %d \n", shoeOfCards->cards[i]->rank,shoeOfCards->cards[i]->suit); */
	/* } */

	uint amountOfCards = sizeof(shoeOfCards->cards) / sizeof(card);

	cardShuffle(shoeOfCards->cards, amountOfCards);

	/* printf("\n"); */
	/* for (int i = 0; i < CARDSINASHOE; i++ ) { */
	/* 	printf("%d %d \n", shoeOfCards->cards[i]->rank,shoeOfCards->cards[i]->suit); */
	/* } */
	shoeOfCards->originalDeck = deckOfCards;
	shoeOfCards->topOfShoe = 0;

	return shoeOfCards;
}

void destroyShoe(shoe *shoePtr){
	free(shoePtr->originalDeck);
	free(shoePtr);
}

card *getCardAtTheTop(shoe *shoePtr) {
	//If the deck has no cards remaining, restart the topOfShoe index and
	//reshuffle the deck
	if (shoePtr->topOfShoe > CARDSINASHOE) {
		shoePtr->topOfShoe = 0;

		//TODO: Make it a struct item instead of macro
		cardShuffle(shoePtr->cards, CARDSINASHOE);
	}

	card *cardAtTheTop = shoePtr->cards[shoePtr->topOfShoe];

	shoePtr->topOfShoe += 1;

	return cardAtTheTop;
}


suits getSuit(card *cardPtr) {
	return cardPtr->suit;
}

int getRank(card *cardPtr) {
	return cardPtr->rank;
}
