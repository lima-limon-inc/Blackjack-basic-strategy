#include <stdio.h>
#include <stdbool.h>

#include "cards.h"
#include "cardArithmetic.h"
#include "player.h"



int main()
{
	printf("Hello world \n");
	shoe *pokerShoe;
	pokerShoe = createShuffledShoe();

	int amountOfCards = sizeof(pokerShoe->cards) / sizeof(card);
	int result = sumCards(pokerShoe->cards, amountOfCards);

	printf("%d \n", result);
	/* printf("%ld \n", sizeof(int)); */

	player *player1 = createPlayer("Hi",  3);
	printf("%s \n", player1->name);
	printf("%d \n", player1->funds);
	printf("%d \n", player1->cardsInHand);
	/* while (true) { */
	/* } */

	destroyShoe(pokerShoe);
	killPlayer(player1);
}
