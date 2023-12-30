#include <assert.h>
#include <fts.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int **read_sudoku(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    return NULL;
  }
  int **matrix = (int **)malloc(sizeof(int *) * 9);
  int rowIndex = 0;
  char line[11];
  while (fgets(line, sizeof(line), file)) {
    int *row = (int *)malloc(sizeof(int) * 9);
    for (int colIndex = 0; colIndex < 9; colIndex++) {
      row[colIndex] = line[colIndex] - '0';
    }
    matrix[rowIndex] = row;
    rowIndex++;
    if (rowIndex == 9)
      break;
  }
  return matrix;
}

void free_matrix(int **matrix) {
  for (int colIndex = 0; colIndex < 9; colIndex++) {
    free(matrix[colIndex]);
  }
  free(matrix);
}

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

int *get_possible_values(int **matrix, int row, int col, int *len) {
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
  int *values = (int *)malloc(sizeof(int) * length);
  int valuesIndex = 0;
  for (int i = 0; i < 9; i++) {
    if (values_array[i] > 0) {
      values[valuesIndex] = values_array[i];
      valuesIndex++;
    }
  }
  *len = length;
  return values;
}

int **solve_sudoku(int **matrix) {
  int row = 0, col = 0;
  get_empty_cell(matrix, &row, &col);
  if (row == -1) {
    // base case, no more empty cells
    return matrix;
  }
  int len = 0;
  int *possible_values = get_possible_values(matrix, row, col, &len);
  if (len == 0) {
    // solution does not exist for this matrix, backtrack
    free(possible_values);
    return NULL;
  }
  for (int i = 0; i < len; i++) {
    int value = possible_values[i];
    matrix[row][col] = value;
    int **solution = solve_sudoku(matrix);
    if (solution != NULL) {
      free(possible_values);
      return solution;
    }
    matrix[row][col] = 0;
  }
  free(possible_values);
  return NULL;
}
int compare(const FTSENT **one, const FTSENT **two) {
  return (strcmp((*one)->fts_name, (*two)->fts_name));
}
int main(int argc, char **argv) {
  char *game_folder = "../games/";
  char *folderPaths[] = {game_folder, NULL};
  int count = 0;
  FTS *folder = fts_open(folderPaths, FTS_PHYSICAL, &compare);
  if (folder != NULL) {
    FTSENT *node = fts_read(folder);
    while (node != NULL) {
      switch (node->fts_info) {
      case FTS_F:
        count++;
      default:
        break;
      }
      node = fts_read(folder);
    }
    fts_close(folder);
  }
  printf("There are %d files\n", count);
  char **game_files = (char **)malloc(sizeof(char *) * count);
  folder = fts_open(folderPaths, FTS_PHYSICAL, &compare);
  if (folder != NULL) {
    FTSENT *node = fts_read(folder);
    int index = 0;
    while (node != NULL) {
      switch (node->fts_info) {
      case FTS_F:
        char *filename =
            (char *)malloc(strlen(game_folder) + strlen(node->fts_name) + 1);
        strncpy(filename, game_folder, strlen(game_folder) + 1);
        strcat(filename, node->fts_name);
        // printf("%s\n", filename);
        game_files[index] = filename;
        index++;
        // printf("%s\n", node->fts_name);
      default:
        break;
      }
      node = fts_read(folder);
    }
    fts_close(folder);
  }
  clock_t start, end;
  double *solving_times = (double *)malloc(sizeof(double) * count);
  for (int i = 0; i < count; i++) {
    int **matrix = read_sudoku(game_files[i]);
    start = clock();
    int **solution = solve_sudoku(matrix);
    end = clock();
    double solving_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    assert(solution != NULL);
    solving_times[i] = solving_time;
    free_matrix(matrix);
  }
  double minTime = solving_times[0];
  double maxTime = solving_times[0];
  double sum = 0.0;
  for (int i = 0; i < count; i++) {
    if (solving_times[i] < minTime) {
      minTime = solving_times[i];
    }
    if (solving_times[i] > maxTime) {
      maxTime = solving_times[i];
    }
    sum += solving_times[i];
  }
  double avgTime = sum / count;
  printf("Execution Times, min: %e s, avg: %e s, max: %e s", minTime, avgTime,
         maxTime);

  for (int i = 0; i < count; i++) {
    free(game_files[i]);
  }
  free(game_files);
  free(solving_times);
  return 0;
}