#include <stdio.h>
#include <stdbool.h>

#include "cards.h"
#include "cardArithmetic.h"
#include "player.h"
#include "dealer.h"



int main()
{
	printf("Hello world \n");
	/* shoe *pokerShoe; */
	/* pokerShoe = createShuffledShoe(); */

	/* int amountOfCards = sizeof(pokerShoe->cards) / sizeof(card); */
	/* int result = sumCards(pokerShoe->cards, amountOfCards); */

	/* printf("%d \n", result); */
	/* printf("%ld \n", sizeof(int)); */

	dealer *dealer1 = createDealer(1000);


	card *newCard = dealACard(dealer1);


	player *player1 = createPlayer("Hi",  3);
	/* printf("%s \n", player1->name); */
	printf("%d \n", dealer1->pokerShoe->topOfShoe);
	printf("%d \n", dealer1->pokerShoe->topOfShoe);
	/* printf("%d \n", player1->cardsInHand); */

	printf("%d \n", newCard->rank);
	/* printf("%d \n", newCard->suit); */
	/* while (true) { */
	/* } */

	/* destroyShoe(pokerShoe); */
	killPlayer(player1);
	killDealer(dealer1);
}
