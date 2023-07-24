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
