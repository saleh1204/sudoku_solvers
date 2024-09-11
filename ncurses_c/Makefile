

all: sudoku_ncurses

run: sudoku_ncurses
	./sudoku_ncurses

sudoku_ncurses: sudoku_ncurses.c sudoku_lib.c
	gcc -c -std=c11 -fsanitize=address -Wall sudoku_lib.c 
	gcc -std=c11 -fsanitize=address -Wall -o sudoku_ncurses sudoku_ncurses.c sudoku_lib.o -lncurses -lpanel

clean:
	rm -rf sudoku_ncurses sudoku_lib.o
