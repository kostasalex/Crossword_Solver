# Crossword Solver
A crossword puzzle generator that takes a text file describing an empty crossword grid and a dictionary of words as input, and outputs a completed crossword puzzle. The program uses various techniques to fill in the grid, such as backtracking and constraint satisfaction.


## Inputs
The program takes in two main inputs:

1. A text file that contains the size of the crossword grid (x * x) and the coordinates of the blocks that cannot place a word (row, column location). 

2. A vocabulary of words to fill the puzzle with. 

In folder /input can be found crossword and vocabulary files.

## Compilation

Run the command make. To remove the object files and the executable, run the command make clean.

## Usage:

To run the program, use the command:
`./crossword [crossword_file] [optional_arguments]`

* crossword_file: Required argument. The name of the text file describing the shape of the crossword puzzle. If no file is provided or if the file is not found in the "inputs" folder, the program will use the default file inputs/crossword.txt.
* optional_arguments:
  * -dict [vocab_file]: Use a specific vocabulary file. If this argument is not provided, the program will use the default file inputs/words.txt.
  * -check: Print all the words in the crossword puzzle from left to right and prompt the user to enter each word. If a word entered by the user does not match the word in the crossword puzzle, the program will terminate.
  * -draw: Print the completed crossword puzzle in the following format (Crossword2.txt):

![crossword_draw](https://user-images.githubusercontent.com/34860262/215159589-30a97057-69ef-4763-b5c5-837af515eb5a.jpg)

## Usage:

The program is run from the command line with the following syntax:
$ ./crossword [crossword file] [options]
Where [crossword file] is the file containing the crossword puzzle and [options] are optional arguments for the program.

The first and mandatory argument is the name of the file containing the crossword puzzle, where the crossword file should be formatted as follows:

## Techniques used

* Backtracking: This is used to find all possible solutions for filling in the crossword puzzle, trying different words in different positions until a valid solution is found.
* Dictionary data structure: A dictionary data structure is used to store and search through the vocabulary words that will be used to fill in the crossword puzzle.
* Constraint satisfaction: The program uses constraints such as the length of the words and the letters that they can intersect with to narrow down the possible solutions and increase the efficiency of the search. Additionally, the program uses forward checking to further constraint the possible solutions by checking the validity of the word choices before they are tried in the backtracking process.

