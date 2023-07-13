#include <stdio.h>
#include <stdbool.h>

#include "cards.h"
#include "cardArithmetic.h"
#include "player.h"
#include "dealer.h"

//TODO: Make dinamic
#define PLAYERAMOUNT 5

int main()
{
	printf("Hello world \n");
	dealer *dealer1 = createDealer(1000);

	player *player1 = createPlayer("Player 1",  100);
	/* player *player2 = createPlayer("Player 2",  100); */
	/* player *player3 = createPlayer("Player 3",  100); */
	/* player *player4 = createPlayer("Player 4",  100); */
	/* player *player5 = createPlayer("Player 5",  100); */

	/* player *everyPlayer[] = { player1, player2, player3, player4, player5 } */




	/* playerPtr->cardsInHand */
	for (int i = 0; i < player1->cardsInHand; i++) {
		printf("%d \n", player1->hand[i]->rank);
	}

	killPlayer(player1);
	killDealer(dealer1);
}
