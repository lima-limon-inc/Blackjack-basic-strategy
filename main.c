#include <stdio.h>
#include "cards.h"
#include <stdarg.h>

/* void myfunc(card *firstCard, ...) */
/* { */
/*    va_list ap; */
/*    /1* int j = 0; *1/ */
/*    card *cardPtr; */

/*    va_start(ap, firstCard); */ 
/*    /1* for(j=0; j<count; j++) *1/ */
/*    for(cardPtr = firstCard; *cardPtr; cardPtr++) */
/*    { */
/*      printf("%d\n", cardPtr->rank); */
/*    } */

/*    va_end(ap); */

/*    /1* return count; *1/ */
/* } */






void main()
{
	/* printf("Hello world \n"); */
	shoe shoeOfCards;
	shoeOfCards = getShuffledShoe();

	/* printf("%d %d \n", shoeOfCards.cards[0]->rank,shoeOfCards.cards[0]->suit); */
}
