
#include "cards.h"

static card *createCard(int rankNumer, suits suitType)
{
	card *newCard = (card *) malloc(sizeof(card));
	newCard->rank = rankNumer;
	newCard->suit = suitType;

	return newCard;
}

static deck createDeckOfCards()
{
	deck deckOfCards;

	for ( suits suitType = Clubs; suitType <= Spades; suitType++) {

		uint offset = DECKLENGTH * suitType;
		for (int rankNumer = 0; rankNumer < DECKLENGTH ; rankNumer++) {
			uint posInArray = rankNumer + offset;

			card *newCard = createCard(rankNumer + 1, suitType);
			deckOfCards.cards[posInArray] = newCard;
		}
	}

	return deckOfCards;
}

static shoe createShoeFromDecks(deck deckOfCards)
{
	shoe shoeOfCards;
	for (int nShoe =0; nShoe < DECKAMOUNT; nShoe++) {

		uint offset = CARDSINADECK * nShoe;
		for (int rankNumer = 0; rankNumer < CARDSINADECK; rankNumer++) {
			uint posInArray = rankNumer + offset;

			card *cardptr = deckOfCards.cards[rankNumer];
			shoeOfCards.cards[posInArray] = cardptr;
		}
	}

	return shoeOfCards;
}

//Fisher-Yates shuffle:
//https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
static void cardShuffle(card *cards[], uint amountOfCards)
{
	/* printf("%d\n", cards[0]->rank); */
	int rankNumer, randomPosition;
	card *tmp;
	for (rankNumer = amountOfCards - 1; rankNumer > 0; rankNumer--) {
		randomPosition = rand() % (rankNumer + 1);
		tmp = cards[randomPosition];
		cards[randomPosition] = cards[rankNumer];
		cards[rankNumer] = tmp;
	}
	/* printf("%d\n", cards[0]->rank); */
}

/* void main() */
shoe getShuffledShoe()
{

	srand(time(NULL));   // Initialization, should only be called once.
	deck deckOfCards;
	deckOfCards = createDeckOfCards();

	/* for (int rankNumer = 0; rankNumer < CARDSINADECK; rankNumer++) { */
	/* 	/1* printf("%d \n", rankNumer); *1/ */
	/* 	printf("%d %d \n", deckOfCards.cards[rankNumer]->rank,deckOfCards.cards[rankNumer]->suit ); */
	/* } */

	shoe shoeOfCards;
	shoeOfCards = createShoeFromDecks(deckOfCards);
	/* for (int rankNumer = 0; rankNumer < CARDSINASHOE; rankNumer++) { */
	/* 	/1* printf("%d \n", rankNumer); *1/ */
	/* 	printf("%d %d \n", shoeOfCards.cards[rankNumer]->rank,shoeOfCards.cards[rankNumer]->suit ); */
	/* } */

	uint amountOfCards;
	amountOfCards = sizeof(shoeOfCards.cards) / sizeof(card);

	cardShuffle(shoeOfCards.cards, amountOfCards);
	/* free(deckOfCards.cards); */

	/* printf("\n"); */
	/* for (int rankNumer = 0; rankNumer < CARDSINASHOE; rankNumer++) { */
	/* 	/1* printf("%d \n", rankNumer); *1/ */
	/* 	printf("%d %d \n", shoeOfCards.cards[rankNumer]->rank,shoeOfCards.cards[rankNumer]->suit ); */
	/* } */
	for (int rankNumer = 0; rankNumer < CARDSINADECK; rankNumer++) {
		/* printf("%d \n", rankNumer); */
		/* printf("%d %d \n", deckOfCards.cards[rankNumer]->rank,deckOfCards.cards[rankNumer]->suit ); */
		free(deckOfCards.cards[rankNumer]);
	}


	printf("%d %d \n", shoeOfCards.cards[0]->rank,shoeOfCards.cards[0]->suit);
	return shoeOfCards;
}
