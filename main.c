#include <stdio.h>
#include <stdbool.h>

#include "cards.h"
#include "cardArithmetic.h"
#include "player.h"



int main()
{
	printf("Hello world \n");
	pokerGame *newPokerGame;
	newPokerGame = createPokerGame();

	/* int amountOfCards = sizeof(newPokerGame->pokerShoe->cards) / sizeof(card); */
	/* int result = sumCards(newPokerGame->pokerShoe->cards, amountOfCards); */

	printf("%d \n", newPokerGame->pokerShoe->cards[0]->rank);
	/* printf("%ld \n", sizeof(int)); */

	/* player *player1 = createPlayer("Hi",  3); */
	/* printf("%d \n", player1->funds); */
	/* printf("%s \n", player1->name); */
	/* while (true) { */
	/* } */
}
