# Overview
*cudoku* is a simple Sudoku solver based on backtracking search. It is written in plain ANSI C and does not use explicit recursion, so it is blazingly fast (despite the brute-force strategy). Originally *cudoku* was a coding kata idea, created while spending about an hour in a doctor's waiting room.

# Usage
*cudoku* must be run from the command line. Example:
```
cudoku puzzle1.txt
```
The argument `puzzle1.txt` is the name of a text file that contains the initial sudoku puzzle, given as a 9 x 9 matrix. For example, the puzzle

| A | B  | C | D | E | F | G | H | I |
|---|----|---|---|---|---|---|---|---|
|   |    | 2 | 4 |   | 8 | 6 |   |   |
|   | 5  |   |   |   |   |   | 1 |   |
| 9 |    |   |   |   |   |   |   | 7 |
| 6 |    |   | 9 |   | 7 |   |   | 5 |
|   |    |   |   |   |   |   |   |   |
| 2 |    |   | 8 |   | 6 |   |   | 9 |
| 5 |    |   |   |   |   |   |   | 6 |
|   | 1  |   |   |   |   |   | 4 |   |
|   |    | 7 | 5 |   | 2 | 1 |   |   |

must be specified in the following form (empty cells are specified using zeroes):
```
1 2 0 0 4 7 0 0 5
0 3 8 0 0 0 7 0 1
0 0 0 1 0 3 0 0 9
8 0 7 4 3 0 6 0 0
0 1 2 0 0 0 0 9 7
0 0 4 7 0 2 5 0 3
0 0 5 0 7 0 1 2 0
7 6 0 9 0 0 0 0 8
2 0 3 8 0 1 9 7 0
```

After solving the puzzle, *cudoku* prints the solution to the standard output:
```
1 2 9 6 4 7 8 3 5
4 3 8 2 9 5 7 6 1
5 7 6 1 8 3 2 4 9
8 5 7 4 3 9 6 1 2
3 1 2 5 6 8 4 9 7
6 9 4 7 1 2 5 8 3
9 8 5 3 7 6 1 2 4
7 6 1 9 2 4 3 5 8
2 4 3 8 5 1 9 7 6
```
