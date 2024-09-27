#include "sudoku_lib.h"
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>

#define GAME_WIDTH 25
#define GAME_HEIGHT 13
#define ERROR_LINE 5

int **matrix;
bool **locked;

int x, y;
uint8_t selected_row = 0, selected_col = 0;
int **read_sudoku(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    return NULL;
  }
  int **matrix = (int **)malloc(sizeof(int *) * 9);
  locked = (bool **)malloc(sizeof(int *) * 9);
  int rowIndex = 0;
  char line[11];
  while (fgets(line, sizeof(line), file)) {
    int *row = (int *)malloc(sizeof(int) * 9);
    bool *row_locked = (bool *)malloc(sizeof(bool) * 9);
    for (int colIndex = 0; colIndex < 9; colIndex++) {
      row[colIndex] = line[colIndex] - '0';

      row_locked[colIndex] = (line[colIndex] > '0' && line[colIndex] <= '9');
    }
    matrix[rowIndex] = row;
    locked[rowIndex] = row_locked;
    rowIndex++;
    if (rowIndex == 9)
      break;
  }
  return matrix;
}

void free_matrix() {
  for (int colIndex = 0; colIndex < 9; colIndex++) {
    free(matrix[colIndex]);
  }
  free(matrix);
}

void free_locked() {
  for (int colIndex = 0; colIndex < 9; colIndex++) {
    free(locked[colIndex]);
  }
  free(locked);
}

void drawSudokuGrid(WINDOW *win, int **matrix, int selected_row,
                    int selected_col) {
  int row_index = 1;
  for (int row = 0; row < 9; row++) {
    char line[24];
    int index = 0;
    line[index] = ' ';
    index++;
    for (int col = 0; col < 9; col++) {
      if (col % 3 == 0 && col != 0) {
        // printf("| ");
        line[index] = '|';
        index++;
        line[index] = ' ';
        index++;
      }
      if (matrix[row][col] == 0) {
        line[index] = '_';
      } else {
        line[index] = matrix[row][col] + '0';
      }
      index++;
      line[index] = ' ';
      index++;
    }
    mvwaddstr(win, row_index, 1, line);
    row_index++;
    if ((row + 1) % 3 == 0 && row != 0 && row != 8) {
      mvwaddstr(win, row_index, 1, " --------------------- ");
      row_index++;
    }
  }
  wmove(win, 1 + selected_row + ((selected_row / 3) * 1),
        2 + (2 * selected_col) + ((selected_col / 3) * 2));
  wrefresh(win);
}

void initWindow(WINDOW *main_win) {
  x = (COLS - GAME_WIDTH) / 2;
  y = (LINES - GAME_HEIGHT) / 2;
  keypad(stdscr, TRUE);
  nonl();   // tell curses not to do NL->CR/NL on output
  cbreak(); // Disables buffering
  noecho(); // tells curses to not echo user input
  clear();  // clears the screen
  start_color();

  /* Initialize all the colors */
  init_pair(1, COLOR_WHITE, COLOR_RED);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);
  init_pair(4, COLOR_CYAN, COLOR_BLACK);
  init_pair(5, COLOR_WHITE, COLOR_BLUE);

  box(main_win, 0, 0);
}

void drawInstructions(WINDOW *main_win) {
  attron(COLOR_PAIR(2));
  mvwaddstr(main_win, 1, 1, "Sudoku Game By Saleh AlSaleh");
  mvwaddstr(main_win, 2, 1, "Press 'q' to exit the game");
  mvwaddstr(main_win, 3, 1,
            "Use the arrow keys (or ijkl) to move to a different cell");
  mvwaddstr(main_win, 4, 1, "Press 's' to solve the game");
  attroff(COLOR_PAIR(2));
  refresh();
}

void updateXY(WINDOW *main_win) {
  char coordinates_str[80];
  attron(A_BOLD);
  attron(COLOR_PAIR(5));
  snprintf(coordinates_str, 80, "x: %d y: %d", selected_row, selected_col);
  mvwaddstr(main_win, LINES - 2, (COLS / 2) - 5, coordinates_str);
  attroff(COLOR_PAIR(5));
  attroff(A_BOLD);
  refresh();
}

void drawGameBorder(WINDOW *main_win, WINDOW *child_win, const char *filename) {
  matrix = read_sudoku(filename);
  if (child_win == NULL) {
    exit(1);
  } else {
    box(child_win, 0, 0);
    wrefresh(child_win);
  }
  updateXY(main_win);
}

void showError(WINDOW *main_win, const char *msg) {
  attron(COLOR_PAIR(1));
  mvwaddstr(main_win, ERROR_LINE, 1, msg);
  attroff(COLOR_PAIR(1));
  refresh();
}
void clearError(WINDOW *main_win) {
  char msg[80];
  for (int i = 0; i < 79; i++) {
    msg[i] = ' ';
  }
  mvwaddstr(main_win, ERROR_LINE, 1, msg);
  // showError (main_win, msg);
}
void handleKeyStrokes(WINDOW *main_win, WINDOW *child_win) {
  int ch;
  while ((ch = getch()) != 'q') {
    clearError(main_win);
    refresh();
    switch (ch) {
    case KEY_DOWN:
    case 'k':
    case 'K':
      selected_row = (selected_row + 1) % 9;
      break;
    case KEY_UP:
    case 'i':
    case 'I':
      selected_row = (selected_row == 0) ? 8 : selected_row - 1;
      break;
    case KEY_RIGHT:
    case 'l':
    case 'L':
      selected_col = (selected_col + 1) % 9;
      break;
    case KEY_LEFT:
    case 'j':
    case 'J':
      selected_col = (selected_col == 0) ? 8 : selected_col - 1;
      break;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      if (matrix[selected_row][selected_col] != 0) {
        showError(main_win, "Error! selected cell already has a value");
        break;
      } else {
        matrix[selected_row][selected_col] = ch - '0';
      }
      break;
    case KEY_DC:
    case KEY_BACKSPACE:
      if (locked[selected_row][selected_col]) {
        showError(main_win, "Error! selected cell value cannot be erased");
        break;
      } else {
        matrix[selected_row][selected_col] = 0;
      }
      break;
    case 's':
    case 'S':
      matrix = solve_sudoku(matrix);
      break;
    default:
      break;
    }
    updateXY(main_win);
    drawSudokuGrid(child_win, matrix, selected_row, selected_col);
  }
}
int main() {
  // Start curses mode
  WINDOW *main_win = initscr();

  // Initialize Main Window
  initWindow(main_win);

  // Draw Game Instructions in text
  drawInstructions(main_win);

  // Create a child window centered in the main window
  WINDOW *child_win = subwin(main_win, GAME_HEIGHT, GAME_WIDTH, y, x);

  // Draw Game border
  drawGameBorder(main_win, child_win, "../games/game00.txt");
  // Draw a Sudoku Game loaded from a predefined file into the game window
  drawSudokuGrid(child_win, matrix, selected_row, selected_col);
  // Main loop function that handles user input and key strokes
  handleKeyStrokes(main_win, child_win);

  // Clearing out data;
  delwin(child_win);
  delwin(main_win);
  free_matrix();
  free_locked();
  endwin(); // End curses mode
  return 0;
}
