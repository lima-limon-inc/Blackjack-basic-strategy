#include <stdio.h>
#include "cards.h"

int main()
{
	printf("Hello world \n");
	shoe shoeOfCards;
	shoeOfCards = getShuffledShoe();

	printf("%d %d \n", shoeOfCards.cards[0]->rank,shoeOfCards.cards[0]->suit);
}
