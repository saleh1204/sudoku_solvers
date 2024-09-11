#include <bits/stdc++.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

std::vector<std::vector<int>> read_sudoku(std::string filename) {
  std::vector<std::vector<int>> matrix(9);
  std::ifstream game_file(filename, std::ios::in);
  std::string line;
  if (!game_file.is_open()) {
    std::cerr << "Error Could not open file" << std::endl;
    return matrix;
  }
  int row_index = 0;
  while (std::getline(game_file, line)) {
    if (line.length() == 0) {
      continue;
    }
    matrix[row_index] = std::vector<int>(9);
    for (int i = 0; i < 9; i++) {
      matrix[row_index][i] = line[i] - 0x30;
    }
    row_index++;
  }
  return matrix;
}
void print_matrix(std::string title, std::vector<std::vector<int>> matrix) {
  if (matrix.size() == 0) {
    return;
  }
  std::cout << title << std::endl;
  for (int row_index = 0; row_index < 9; row_index++) {
    for (int col_index = 0; col_index < 9; col_index++) {
      if (col_index % 3 == 0 && (col_index != 0)) {
        std::cout << "| ";
      }
      std::cout << matrix[row_index][col_index] << " ";
    }
    std::cout << std::endl;
    if ((row_index + 1) % 3 == 0 && (row_index != 0) && (row_index != 8)) {
      std::cout << "------+-------+------" << std::endl;
    }
  }
}

void get_empty_cell(std::vector<std::vector<int>> matrix, int *row, int *col) {
  for (int row_index = 0; row_index < 9; row_index++) {
    for (int col_index = 0; col_index < 9; col_index++) {
      if (matrix[row_index][col_index] == 0) {
        *row = row_index;
        *col = col_index;
        return;
      }
    }
  }
  *row = -1;
  *col = -1;
}
std::vector<int> get_possible_values(std::vector<std::vector<int>> matrix,
                                     int row, int col) {
  std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  for (auto element : matrix[row]) {
    auto it = std::find(values.begin(), values.end(), element);
    if (it != values.end()) {
      values.erase(it);
    }
  }

  for (int row_index = 0; row_index < 9; row_index++) {
    auto element = matrix[row_index][col];
    auto it = std::find(values.begin(), values.end(), element);
    if (it != values.end()) {
      values.erase(it);
    }
  }

  int block_no = ((row / 3) * 3) + (col / 3);
  int block_row_index = (block_no / 3) * 3;
  int block_col_index = (block_no % 3) * 3;

  for (int row_index = block_row_index; row_index < block_row_index + 3;
       row_index++) {
    for (int col_index = block_col_index; col_index < block_col_index + 3;
         col_index++) {
      auto element = matrix[row_index][col_index];
      auto it = std::find(values.begin(), values.end(), element);
      if (it != values.end()) {
        values.erase(it);
      }
    }
  }
  return values;
}
std::vector<std::vector<int>>
solve_sudoku(std::vector<std::vector<int>> matrix) {
  int row = 0, col = 0;
  get_empty_cell(matrix, &row, &col);
  if (row == -1) {
    return matrix;
  }
  std::vector<int> possible_values = get_possible_values(matrix, row, col);
  if (possible_values.size() == 0) {
    return std::vector<std::vector<int>>(0);
  }
  for (int value : possible_values) {
    matrix[row][col] = value;
    std::vector<std::vector<int>> solution = solve_sudoku(matrix);
    if (solution.size() != 0) {
      return solution;
    }
    matrix[row][col] = 0;
  }
  return std::vector<std::vector<int>>(0);
}
int main(void) {
  std::vector<double> solving_times;
  for (auto &file :
       std::filesystem::recursive_directory_iterator("../games/")) {
    // std::cout << file << std::endl;
    std::vector<std::vector<int>> matrix = read_sudoku(file.path());
    //   print_matrix("Before Solving", matrix);
    auto start = std::chrono::high_resolution_clock::now();
    //   print_matrix("After Solving", solved);
    std::vector<std::vector<int>> solved = solve_sudoku(matrix);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> us_double = end - start;
    solving_times.push_back(us_double.count());
  }
  const auto [min_time, max_time] =
      std::minmax_element(begin(solving_times), end(solving_times));
  double sum = std::accumulate(solving_times.begin(), solving_times.end(), 0);
  std::cout << "Execution Times, min: " << *min_time
            << " us, avg: " << (sum / solving_times.size())
            << " us, max: " << *max_time << " us" << std::endl;
  return 0;
}