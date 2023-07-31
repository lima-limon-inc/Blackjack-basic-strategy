#ifndef BASICSTRATEGY_HEADER
#define BASICSTRATEGY_HEADER

#include <stdbool.h>

#include "playerDecision.h"
#include "cards.h"
#include "playerHand.h"

playerDecision getCorrectChoice(playerHand *playersHand, card *dealersCard);

#endif
