#ifndef SUDOKU_LIB_H
#define SUDOKU_LIB_H

#include <stddef.h>
#include <stdio.h>

void print_matrix(char *title, int **matrix);
void get_empty_cell(int **matrix, int *row, int *col);
void get_possible_values(int **matrix, int row, int col, int *values, int *len);
int **solve_sudoku(int **matrix);

#endif