
package com.saleh;

import java.io.File;
import java.io.IOException;
import java.time.Duration;
import java.time.Instant;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;
import java.util.stream.LongStream;

public class App {
  public static int[][] read_sudoku(String filename) {
    int[][] matrix = new int[9][9];
    File file = new File(filename);
    try {
      //   FileInputStream stream = new FileInputStream(file);
      Scanner scanner = new Scanner(file);
      for (int i = 0; i < 9; i++) {
        String[] chars = scanner.nextLine().split("");
        for (int j = 0; j < 9; j++) {
          matrix[i][j] = Integer.parseInt(chars[j]);
        }
      }
      scanner.close();
      //   stream.r
    } catch (IOException e) {
      System.err.println("Error! file does not exist");
    }

    return matrix;
  }

  public static void print_matrix(String title, int[][] matrix) {
    if (matrix == null) {
      return;
    }
    System.out.println(title);
    for (int row_index = 0; row_index < 9; row_index++) {
      for (int col_index = 0; col_index < 9; col_index++) {
        if (col_index % 3 == 0 && col_index != 0) {
          System.out.print("| ");
        }
        System.out.print(matrix[row_index][col_index] + " ");
      }
      System.out.println();
      if ((row_index + 1) % 3 == 0 && row_index != 0 && row_index != 8) {
        System.out.println("---------------------");
      }
    }
  }
  public static int[] get_empty_cell(int[][] matrix) {
    int[] result = {-1, -1};
    for (int row_index = 0; row_index < 9; row_index++) {
      for (int col_index = 0; col_index < 9; col_index++) {
        if (matrix[row_index][col_index] == 0) {
          result[0] = row_index;
          result[1] = col_index;
          return result;
        }
      }
    }
    return result;
  }
  public static int[] get_possible_values(int[][] matrix, int row, int col) {
    int[] values_arr = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int length = 9;
    int[] current_row = matrix[row];
    for (int col_index = 0; col_index < 9; col_index++) {
      int element = current_row[col_index];
      if (element != 0) {
        for (int i = 0; i < 9; i++) {
          if (element == values_arr[i]) {
            values_arr[i] = -1;
            length--;
          }
        }
      }
    }
    for (int row_index = 0; row_index < 9; row_index++) {
      int element = matrix[row_index][col];
      if (element != 0) {
        for (int i = 0; i < 9; i++) {
          if (element == values_arr[i]) {
            values_arr[i] = -1;
            length--;
          }
        }
      }
    }
    int block_no = ((row / 3) * 3) + (col / 3);
    int block_row_index = (block_no / 3) * 3;
    int block_col_index = (block_no % 3) * 3;
    for (int row_index = block_row_index; row_index < block_row_index + 3;
         row_index++) {
      for (int col_index = block_col_index; col_index < block_col_index + 3;
           col_index++) {
        int element = matrix[row_index][col_index];
        if (element != 0) {
          for (int i = 0; i < 9; i++) {
            if (element == values_arr[i]) {
              values_arr[i] = -1;
              length--;
            }
          }
        }
      }
    }
    int[] result = new int[length];
    int j = 0;
    for (int i = 0; i < 9; i++) {
      if (values_arr[i] != -1) {

        result[j] = values_arr[i];
        j++;
      }
    }
    return result;
  }

  public static int[][] solve_sudoku(int[][] matrix) {
    int[] row_col = get_empty_cell(matrix);
    int row = row_col[0], col = row_col[1];
    if (row == -1) {
      // base case, no more empty cells
      return matrix;
    }
    int[] possible_values = get_possible_values(matrix, row, col);
    if (possible_values.length == 0) {
      // solution does not exist for matrix, backtrack
      return null;
    }
    for (int i = 0; i < possible_values.length; i++) {
      int value = possible_values[i];
      matrix[row][col] = value;
      int[][] solution = solve_sudoku(matrix);
      if (solution != null) {
        return solution;
      }
      matrix[row][col] = 0;
    }
    return null;
  }
  public static void main(String[] args) {
    File games_folder = new File("../../games/");
    File[] game_files = games_folder.listFiles();
    if (game_files != null) {
      ArrayList<Long> solving_times = new ArrayList<Long>();
      for (File game_file : game_files) {
        int[][] matrix = read_sudoku(game_file.toString());
        // print_matrix("Before Solving", matrix);
        Instant start = Instant.now();
        int[][] solved = solve_sudoku(matrix);
        Instant finish = Instant.now();
        assert (solved != null);
        long timeElapsed = Duration.between(start, finish).toNanos();
        solving_times.add(timeElapsed);
        // System.out.printf("Execution Time: %d\n", timeElapsed);
        assert (solved != null);
        // print_matrix("After Solving", solved);
      }

      long min_time = solving_times.get(0);
      long max_time = solving_times.get(0);
      long sum = 0;
      for (long value : solving_times) {
        sum += value;
        if (value < min_time) {
          min_time = value;
        }
        if (value > max_time) {
          max_time = value;
        }
      }
      long avg_time = sum / solving_times.size();
      double min_time_s = min_time * 1.0 / 1E9;
      double max_time_s = max_time * 1.0 / 1E9;
      double avg_time_s = avg_time * 1.0 / 1E9;

      System.out.printf("Execution Times, min: %e s, avg: %e s, max: %e s",
                        min_time_s, avg_time_s, max_time_s);
    }
  }
}
