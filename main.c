#include <stdio.h>
#include <stdbool.h>

#include "cards.h"
/* #include "cardArithmetic.h" */
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
	/* printf(RED "♣\n"     RESET); */
	/* printf(GRN "♦\n"   RESET); */
	/* printf(YEL "♥\n"  RESET); */
	/* printf(BLU "♠\n"    RESET); */
	/* printf(MAG "magenta\n" RESET); */
	/* printf(CYN "cyan\n"    RESET); */
	/* printf(WHT "white\n"   RESET); */

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

	destroyPokerTable(newPokerTable);
}
