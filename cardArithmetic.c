#include <stdarg.h>
#include <stdlib.h>
#include "cards.h"

uint sumCards(card *cards[], int amountOfCards)
{
	int sum = 0;
	for (int card = 0; card < amountOfCards; card++) {
		sum += cards[card]->rank;
	}

   return sum;
}
