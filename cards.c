#include "cards.h"

static deck createDeckOfCards()
{
	deck deckOfCards;

	for ( suits suitType = Clubs; suitType <= Spades; suitType++) {

		uint offset = DECKLENGTH * suitType;
		for (int rankNumer = 0; rankNumer < DECKLENGTH ; rankNumer++) {
			uint posInArray = rankNumer + offset;

			/* card *newCard = createCard(rankNumer + 1, suitType); */
			card newCard = { .rank = rankNumer + 1, .suit = suitType };
			deckOfCards.cards[posInArray] = newCard;
		}
	}

	return deckOfCards;
}

static shoe createShoeFromDecks(deck *deckOfCards)
{
	shoe shoeOfCards;
	for (int nShoe =0; nShoe < DECKAMOUNT; nShoe++) {

		uint offset = CARDSINADECK * nShoe;
		for (int rankNumer = 0; rankNumer < CARDSINADECK; rankNumer++) {
			uint posInArray = rankNumer + offset;

			card *cardptr = &deckOfCards->cards[rankNumer];
			shoeOfCards.cards[posInArray] = cardptr;
		}
	}

	return shoeOfCards;
}

//Fisher-Yates shuffle:
//https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
static void cardShuffle(card *cards[], uint amountOfCards)
{
	int rankNumer, randomPosition;
	card *tmp;
	for (rankNumer = amountOfCards - 1; rankNumer > 0; rankNumer--) {
		randomPosition = rand() % (rankNumer + 1);
		tmp = cards[randomPosition];
		cards[randomPosition] = cards[rankNumer];
		cards[rankNumer] = tmp;
	}
}

shoe getShuffledShoe()
{

	srand(time(NULL));   // Initialization, should only be called once.
	deck deckOfCards = createDeckOfCards();

	/* for (int i = 0; i < CARDSINADECK; i++ ) { */
	/* 	printf("%d %d \n", deckOfCards.cards[i].rank,deckOfCards.cards[i].suit); */
	/* } */

	shoe shoeOfCards = createShoeFromDecks(&deckOfCards);

	/* for (int i = 0; i < CARDSINASHOE; i++ ) { */
	/* 	printf("%d %d \n", shoeOfCards.cards[i]->rank,shoeOfCards.cards[i]->suit); */
	/* } */

	uint amountOfCards = sizeof(shoeOfCards.cards) / sizeof(card);

	cardShuffle(shoeOfCards.cards, amountOfCards);

	/* printf("\n"); */
	/* for (int i = 0; i < CARDSINASHOE; i++ ) { */
	/* 	printf("%d %d \n", shoeOfCards.cards[i]->rank,shoeOfCards.cards[i]->suit); */
	/* } */

	return shoeOfCards;
}
