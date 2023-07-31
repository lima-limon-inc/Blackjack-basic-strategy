#include <stdio.h>
#include <stdbool.h>

#include "cards.h"
/* #include "cardArithmetic.h" */
#include "globalMacros.h"
#include "player.h"
#include "dealer.h"
#include "blackjackTable.h"

//TODO: Make dinamic
#define PLAYERAMOUNT 5


int main()
{
	blackjackTable *newBlackjackTable = createBlackjackTable(1000);
	char name[20];
	printf("Enter the name of the players.\nWrite '0' when done\n");
	while (true) {
		scanf("%s",name);
		FLUSHSTDIN;
		/* if (newBlackjackTable->playerAmount == PLAYERAMOUNT) { */
		/* 	break; */
		/* } */
		if (strcmp(name,"0") == 0) {
			break;
		}
		newBlackjackTable = addPlayer(newBlackjackTable, name, 100);
	}

	blackjackRound(newBlackjackTable);

	destroyBlackjackTable(newBlackjackTable);
}
