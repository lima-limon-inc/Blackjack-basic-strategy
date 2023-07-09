#include <stdarg.h>
#include <stdlib.h>
#include "cards.h"

uint sumCards(card *cards[], int amountOfCards)
{
	/* printf("%d\n", amountOfCards); */
	/* printf("%d\n", (*cards)[0].rank); */

	int sum = 0;
	for (int card = 0; card < amountOfCards; card++) {
		/* printf("%d\n", cards[card]->rank); */
		sum += cards[card]->rank;
	}
	printf("%d\n", sum);

   return sum;
}
