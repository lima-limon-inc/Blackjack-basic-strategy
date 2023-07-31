#include <stdio.h>
#include <stdbool.h>

#include "cards.h"
/* #include "cardArithmetic.h" */
#include "player.h"
#include "dealer.h"
#include "blackjackTable.h"

//TODO: Make dinamic
#define PLAYERAMOUNT 5


int main()
{
	blackjackTable *newBlackjackTable = createBlackjackTable(1000);
	char name[20];
	while (scanf("%s",name) == 1) {
		/* if (newBlackjackTable->playerAmount == PLAYERAMOUNT) { */
		/* 	break; */
		/* } */
		if (strcmp(name,"No") == 0) {
			break;
		}
		newBlackjackTable = addPlayer(newBlackjackTable, name, 100);
	}

	blackjackRound(newBlackjackTable);

	destroyBlackjackTable(newBlackjackTable);
}
