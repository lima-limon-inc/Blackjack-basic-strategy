#include <stdio.h>
#include <stdbool.h>

#include "cards.h"
/* #include "cardArithmetic.h" */
#include "globalMacros.h"
#include "player.h"
#include "dealer.h"
#include "blackjackTable.h"
#include "graphics.h"

//TODO: Make dinamic
#define PLAYERAMOUNT 5


int main()
{
	printCasinoWelcomeSign();
	blackjackTable *newBlackjackTable = createBlackjackTable(1000);
	char name[20];
	printf("Enter the name of the players.\nWrite '%s' when done\n", ENOUGHSYMBOLSTRING);
	while (true) {
		scanf("%s",name);
		FLUSHSTDIN;
		/* if (newBlackjackTable->playerAmount == PLAYERAMOUNT) { */
		/* 	break; */
		/* } */
		if (strcmp(name,ENOUGHSYMBOLSTRING) == ENOUGHSYMBOL) {
			break;
		}
		newBlackjackTable = addPlayer(newBlackjackTable, name, 100);
	}

	blackjackRound(newBlackjackTable);

	destroyBlackjackTable(newBlackjackTable);
}
