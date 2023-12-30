// use std::vec;
use array2d::{Array2D, Error};
use std::fs;
use std::fs::read_to_string;
use std::time::{Duration, Instant};
fn read_sudoku(filename: &str) -> Result<Array2D<i32>, Error> {
    let mut rows: Vec<Vec<i32>> = Vec::new();
    for line in read_to_string(filename).unwrap().lines() {
        let mut row: Vec<i32> = Vec::new();
        for c in line.chars() {
            row.push(c as i32 - 0x30);
        }
        rows.push(row);
    }
    let matrix = Array2D::from_rows(&rows);
    // return matrix;
    return Ok(matrix?);
}

// fn print_matrix(title: &str, matrix: Array2D<i32>) {
//     println!("{}", title);
//     for row_index in 0..9 {
//         for col_index in 0..9 {
//             if col_index % 3 == 0 && col_index != 0 {
//                 print!("| ");
//             }
//             print!("{} ", matrix[(row_index, col_index)]);
//         }
//         println!("");
//         if (row_index + 1) % 3 == 0 && row_index != 0 && row_index != 8 {
//             println!("---------------------");
//         }
//     }
// }
fn get_empty_cell(matrix: Array2D<i32>) -> (i32, i32) {
    for row_index in 0..9 {
        for col_index in 0..9 {
            if matrix[(row_index, col_index)] == 0 {
                return (row_index as i32, col_index as i32);
            }
        }
    }

    return (-1, -1);
}
fn get_possible_values(matrix: Array2D<i32>, row: i32, col: i32) -> Vec<i32> {
    // check the values of the current row
    let mut values: Vec<i32> = vec![1, 2, 3, 4, 5, 6, 7, 8, 9];
    for col_index in 0..9 {
        let element = matrix[(row as usize, col_index)];
        if element != 0 {
            values.retain(|&x| x != element);
        }
    }
    // check the values of the current column
    for row_index in 0..9 {
        let element = matrix[(row_index, col as usize)];
        if element != 0 {
            values.retain(|&x| x != element);
        }
    }
    let block_no = ((row / 3) * 3) + (col / 3);
    let block_row_index = (block_no / 3) * 3;
    let block_col_index = (block_no % 3) * 3;

    for row_index in block_row_index..block_row_index + 3 {
        for col_index in block_col_index..block_col_index + 3 {
            let element = matrix[(row_index as usize, col_index as usize)];
            if element != 0 {
                values.retain(|&x| x != element);
            }
        }
    }
    return values;
}

fn solve_sudoku(mut matrix: Array2D<i32>) -> Array2D<i32> {
    let (row, col) = get_empty_cell(matrix.clone());
    if row == -1 {
        return matrix;
    }
    let empty_matrix: Array2D<i32> = Array2D::filled_with(0, 1, 1);
    let possible_values = get_possible_values(matrix.clone(), row, col);
    if possible_values.len() == 0 {
        return empty_matrix;
    }
    for value in &possible_values {
        let _ = matrix.set(row as usize, col as usize, *value);
        // matrix[(row as usize, col as usize)] = *value;
        let solution = solve_sudoku(matrix.clone());
        if solution.row_len() != 1 {
            return solution;
        }
        let _ = matrix.set(row as usize, col as usize, 0);

        // matrix[(row as usize, col as usize)] = 0;
    }
    return empty_matrix;
}
fn main() {
    let mut solving_times: Vec<Duration> = vec![];
    let paths = fs::read_dir("../games/").unwrap();
    for path in paths {
        // println!("Name: {}", path.unwrap().path().display())
        let matrix = read_sudoku(&path.unwrap().path().display().to_string()).unwrap();
        // print_matrix("Before Solving", matrix.clone());
        let start = Instant::now();
        let solution = solve_sudoku(matrix.clone());
        let duration = start.elapsed();
        assert!(solution.row_len() == 9);
        solving_times.push(duration);
        // print_matrix("After Solving", solved);
    }

    let min_time = solving_times.iter().min().unwrap();
    let avg_time: Duration = solving_times.iter().sum();
    let max_time = solving_times.iter().max().unwrap();
    let min_time_s: f64 = min_time.as_nanos() as f64 / 1E9 as f64;
    let avg_time_s: f64 = avg_time.as_nanos() as f64 / (solving_times.len() as f64 * 1E9 as f64);
    let max_time_s: f64 = max_time.as_nanos() as f64 / 1E9 as f64;
    println!(
        "Execution Times, min: {:+e}, avg: {:+e}, max: {:+e}",
        min_time_s, avg_time_s, max_time_s
    );
}
