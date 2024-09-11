#include "sudoku_lib.h"

void print_matrix(char *title, int **matrix) {
  if (matrix == NULL)
    return;
  printf("%s\n", title);
  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 9; col++) {
      if (col % 3 == 0 && col != 0) {
        printf("| ");
      }
      printf("%d ", matrix[row][col]);
    }
    printf("\n");
    if ((row + 1) % 3 == 0 && row != 0 && row != 8) {
      printf("---------------------\n");
    }
  }
}

void get_empty_cell(int **matrix, int *row, int *col) {
  for (int rowIndex = 0; rowIndex < 9; rowIndex++) {
    for (int colIndex = 0; colIndex < 9; colIndex++) {
      if (matrix[rowIndex][colIndex] == 0) {
        *row = rowIndex;
        *col = colIndex;
        return;
      }
    }
  }
  *row = -1;
  *col = -1;
  return;
}

void get_possible_values(int **matrix, int row, int col, int *values,
                         int *len) {
  int values_array[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  int length = 9;
  int *currentRow = matrix[row];
  // check the values of the current row
  for (int colIndex = 0; colIndex < 9; colIndex++) {
    int element = currentRow[colIndex];
    if (element != 0) {
      for (int i = 0; i < 9; i++) {
        if (element == values_array[i]) {
          values_array[i] = -1;
          length--;
        }
      }
    }
  }
  // check the values of the current column
  for (int rowIndex = 0; rowIndex < 9; rowIndex++) {
    int element = matrix[rowIndex][col];
    if (element != 0) {
      for (int i = 0; i < 9; i++) {
        if (element == values_array[i]) {
          values_array[i] = -1;
          length--;
        }
      }
    }
  }
  // check the values of the current block
  int block_no = ((row / 3) * 3) + (col / 3);
  int block_row_index = (block_no / 3) * 3;
  int block_col_index = (block_no % 3) * 3;
  for (int rowIndex = block_row_index; rowIndex < block_row_index + 3;
       rowIndex++) {
    for (int colIndex = block_col_index; colIndex < block_col_index + 3;
         colIndex++) {
      int element = matrix[rowIndex][colIndex];
      if (element != 0) {
        for (int i = 0; i < 9; i++) {
          if (element == values_array[i]) {
            values_array[i] = -1;
            length--;
          }
        }
      }
    }
  }
  int valuesIndex = 0;
  for (int i = 0; i < 9; i++) {
    if (values_array[i] > 0) {
      values[valuesIndex] = values_array[i];
      valuesIndex++;
    }
  }
  *len = length;
}

int **solve_sudoku(int **matrix) {
  int row = 0, col = 0;
  get_empty_cell(matrix, &row, &col);
  if (row == -1) {
    // base case, no more empty cells
    return matrix;
  }
  int len = 0;
  int possible_values[9];
  get_possible_values(matrix, row, col, possible_values, &len);
  if (len == 0) {
    // solution does not exist for this matrix, backtrack
    return NULL;
  }
  for (int i = 0; i < len; i++) {
    int value = possible_values[i];
    matrix[row][col] = value;
    int **solution = solve_sudoku(matrix);
    if (solution != NULL) {
      return solution;
    }
    matrix[row][col] = 0;
  }
  return NULL;
}