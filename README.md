# Blackjack Basic Strategy

## What the program does

Play Blackjack with your friends (locally) right on the terminal!

Each time a move is made, the program will let you know if the decision you made is the correct one, according to the Blackjack Basic Strategy chart.

## How to install
Just clone the repository and type in:
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
1: Or so it seems!
