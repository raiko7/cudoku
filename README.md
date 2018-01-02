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
0 0 2 4 0 8 6 0 0
0 5 0 0 0 0 0 1 0
9 0 0 0 0 0 0 0 7
6 0 0 9 0 7 0 0 5
0 0 0 0 0 0 0 0 0
2 0 0 8 0 6 0 0 9
5 0 0 0 0 0 0 0 6
0 1 0 0 0 0 0 4 0
0 0 7 5 0 2 1 0 0
```

After solving the puzzle, *cudoku* prints the solution to the standard output:
```
1 7 2 4 9 8 6 5 3
8 5 6 2 7 3 9 1 4
9 4 3 6 5 1 2 8 7
6 8 1 9 4 7 3 2 5
7 9 4 3 2 5 8 6 1
2 3 5 8 1 6 4 7 9
5 2 9 1 8 4 7 3 6
3 1 8 7 6 9 5 4 2
4 6 7 5 3 2 1 9 8
```
