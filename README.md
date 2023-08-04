# Blackjack Basic Strategy

## What the program does

Play Blackjack with your friends (locally) right on the terminal!

Each time a move is made, the program will let you know if the decision you made is the correct one, according to the Blackjack Basic Strategy chart.

## Blackjack rules
- You can hit and stand as long as you haven't bust
- You can double down only in your first hand
- You can split your cards (4 splits max [This is fairly standard])
- Dealer stops at soft 17
- You can ask for insurance if the dealer has an ace (No insurance is offered with a 10)
    - Insurance is worth half your bet
- If the dealer has Blackjack, play is suspended (If a player did ask for insurance, they'll be rewarded)
- There's no surrender (Although it could be implemented)

## How to install
Just clone the repository and run:
```sh
make
```

## Charts
### Hard Hands
|    |  2 | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | A  |
|----|----|----|----|----|----|----|----|----|----|----|
| 5  | HT | HT | HT | HT | HT | HT | HT | HT | HT | HT |
| 6  | HT | HT | HT | HT | HT | HT | HT | HT | HT | HT |
| 7  | HT | HT | HT | HT | HT | HT | HT | HT | HT | HT |
| 8  | HT | HT | HT | HT | HT | HT | HT | HT | HT | HT |
| 9  | HT | DD | DD | DD | DD | HT | HT | HT | HT | HT |
|10  | DD | DD | DD | DD | DD | DD | DD | DD | HT | HT |
|11  | DD | DD | DD | DD | DD | DD | DD | DD | DD | DD |
|12  | HT | HT | ST | ST | ST | HT | HT | HT | HT | HT |
|13  | ST | ST | ST | ST | ST | HT | HT | HT | HT | HT |
|14  | ST | ST | ST | ST | ST | HT | HT | HT | HT | HT |
|15  | ST | ST | ST | ST | ST | HT | HT | HT | HT | HT |
|16  | ST | ST | ST | ST | ST | HT | HT | HT | HT | HT |
|17  | ST | ST | ST | ST | ST | ST | ST | ST | ST | ST |
|18  | ST | ST | ST | ST | ST | ST | ST | ST | ST | ST |
|19  | ST | ST | ST | ST | ST | ST | ST | ST | ST | ST |
|20  | ST | ST | ST | ST | ST | ST | ST | ST | ST | ST |
|21  | ST | ST | ST | ST | ST | ST | ST | ST | ST | ST |

### Soft Hands
|   |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 | A  |
|---|----|----|----|----|----|----|----|----|----|----|
|A,2| HT | HT | HT | DD | DD | HT | HT | HT | HT | HT |
|A,3| HT | HT | HT | DD | DD | HT | HT | HT | HT | HT |
|A,4| HT | HT | DD | DD | DD | HT | HT | HT | HT | HT |
|A,5| HT | HT | DD | DD | DD | HT | HT | HT | HT | HT |
|A,6| HT | DD | DD | DD | DD | HT | HT | HT | HT | HT |
|A,7| ST | DD | DD | DD | DD | ST | ST | HT | HT | HT |
|A,8| ST | ST | ST | ST | ST | ST | ST | ST | ST | ST |
|A,9| ST | ST | ST | ST | ST | ST | ST | ST | ST | ST |

### Split hands
|     | 2  |3   |4   |5   |6   |7   |8   |9   |10  |A  |
|-----|----|----|----|----|----|----|----|----|----|---|
| 2,2 | SP | SP | SP | SP | SP | SP | HT | HT | HT | HT|
| 3,3 | SP | SP | SP | SP | SP | SP | HT | HT | HT | HT|
| 4,4 | HT | HT | HT | SP | SP | HT | HT | HT | HT | HT|
| 5,5 | DD | DD | DD | DD | DD | DD | DD | DD | HT | HT|
| 6,6 | SP | SP | SP | SP | SP | HT | HT | HT | HT | HT|
| 7,7 | SP | SP | SP | SP | SP | SP | HT | HT | HT | HT|
| 8,8 | SP | SP | SP | SP | SP | SP | SP | SP | SP | SP|
| 9,9 | SP | SP | SP | SP | SP | ST | SP | SP | SP | SP|
| T,T | ST | ST | ST | ST | ST | ST | ST | ST | ST | ST|
| A,A | SP | SP | SP | SP | SP | SP | SP | SP | SP | SP|
};


## TODO:
- [ ] Make it so that the program takes into account the running/true count, in order to practice [Card Counting](https://en.wikipedia.org/wiki/Card_counting)
    - This shouldn't be too hard to do

## Asterisks:
1: This is the first time I write anything in C. The code isn't the cleanest. I'll probably refactor stuff later on.
There are messy things in the source code. I just wanted to finish it
