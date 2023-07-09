#include <stdio.h>
#include "cards.h"
#include "cardArithmetic.h"



int main()
{
	printf("Hello world \n");
	shoe shoeOfCards;
	shoeOfCards = getShuffledShoe();

	int amountOfCards = sizeof(shoeOfCards.cards) / sizeof(card);
	int result = sumCards(shoeOfCards.cards, amountOfCards);

	printf("%d \n", result);
	printf("%ld \n", sizeof(int));
}
