#ifndef BASICSTRATEGY_HEADER
#define BASICSTRATEGY_HEADER

#include <stdbool.h>

#include "playerDecision.h"
#include "cards.h"

bool isCorrectChoice(playerDecision decisionToCheck, card *playersCard[], int amountOfCards, card *dealersCard);

#endif
