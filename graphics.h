#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

#include <stdbool.h>

#include "cards.h"
#include "playerDecision.h"
#include "playerHand.h"

void asciiRepresentation(playerHand *playersHand, bool lastCardBlank);

void printCorrectOrNot(bool isCorrect, playerDecision correctDecision);

#endif
