#include <stdio.h>
#include <stdbool.h>

#include "cards.h"
#include "cardArithmetic.h"
#include "player.h"
#include "dealer.h"
#include "pokerTable.h"

//TODO: Make dinamic
#define PLAYERAMOUNT 5

int main()
{
	/* printf("Hello world \n"); */
	/* printf("%ld \n", sizeof(card)); */
	/* printf("%ld \n", sizeof(card *)); */

	pokerTable *newPokerTable = createPokerTable(1000);
	char name[20];
	while (scanf("%s",name) == 1) {
		if (newPokerTable->playerAmount == PLAYERAMOUNT) {
			break;
		}
		if (strcmp(name,"No") == 0) {
			break;
		}
		newPokerTable = addPlayer(newPokerTable, name, 100);
	}

	pokerRound(newPokerTable);



		/* scanf("%s",c); */
	/* player *player1 = createPlayer(c,  100); */

	/* scanf("%s",c); */
	/* } */
	/* dealer *dealer1 = createDealer(1000); */


	/* addPlayer(newPokerTable, player1); */
	
	destroyPokerTable(newPokerTable);
	/* player *player2 = createPlayer("Player 2",  100); */
	/* player *player3 = createPlayer("Player 3",  100); */
	/* player *player4 = createPlayer("Player 4",  100); */
	/* player *player5 = createPlayer("Player 5",  100); */

	/* player *everyPlayer[] = { player1, player2, player3, player4, player5 } */




	/* playerPtr->cardsInHand */
	/* for (int i = 0; i < player1->cardsInHand; i++) { */
	/* } */

	/* killPlayer(player1); */
	/* killDealer(dealer1); */
}
