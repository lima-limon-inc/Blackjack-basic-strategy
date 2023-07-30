#ifndef BASICSTRATEGY_HEADER
#define BASICSTRATEGY_HEADER

#include <stdbool.h>

#include "playerDecision.h"
#include "cards.h"

playerDecision getCorrectChoice(card *playersCard[], int amountOfCards, card *dealersCard);

#endif
