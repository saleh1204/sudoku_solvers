package com.saleh;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

/**
 * Unit test for simple App.
 */
public class AppTest {
  /**
   * Rigorous Test :-)
   */
  @Test
  public void shouldAnswerWithTrue() {
    int[][] matrix = App.read_sudoku("../../games/game00.txt");
    App.print_matrix("Before Solving", matrix);
    int[][] solved = App.solve_sudoku(matrix);
    App.print_matrix("After Solving", solved);
    assertTrue(solved != null);
  }
}
