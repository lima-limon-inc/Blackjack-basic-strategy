#include "cards.h"

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

//amountOfCards represents how many of the cards are valid cards.
//The array may also contain old, values from previous rounds. Those are not 
//meant to be counted. Normally, amountOfCards will be equal to the amount of
//cards in a players hand
int sumCards(const card *cards[], int amountOfCards)
{
	int sum = 0;
	bool foundAnAce = false;

	for (int card = 0; card < amountOfCards; card++) {
		int cardValue = cards[card]->rank;

		//Only one ace can be used with value 11
		if (cardValue == 1 && foundAnAce == false) {
			foundAnAce = true;
			cardValue = 11;
		}

		//King, Queen, Jack all equal 10
		if (cardValue > 10) {
			cardValue = 10;
		}

		sum += cardValue;
	}

	//If the sum of all the cards is greater than 21 BECAUSE of an ace,
	//then we substract 10. Because the other value of the ace is 1
	//11 - 10 = 1
	if (foundAnAce == true && sum > 21) {
		sum -= 10;
	}

   return sum;
}
