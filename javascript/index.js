const fs = require('fs');
var deasync = require('deasync');
var path = require('path');
const { assert } = require('console');
function read_sudoku(filename) {
    let matrix = [];
    var done = false;
    fs.open(filename, "r", function (err, fd) {
        if (err) {
            return console.error(err);
        }
        fs.read(fd, function (err, bytesLen, buffer) {
            if (err) {
                return console.error(err);
            }
            contents = buffer.slice(0, bytesLen).toString();
            let row = 0;
            contents.split("\n").forEach(line => {
                if (line != "") {
                    matrix[row] = [];
                    for (var col = 0; col < line.length; col++) {
                        matrix[row][col] = line[col] - '0';

                    }
                    row++;
                }
            });
            done = true;
        });
    })
    while (!done) {
        deasync.sleep(100);
    }
    return matrix;
}

function print_matrix(title, matrix) {
    console.log(title);
    for (var row_index = 0; row_index < 9; row_index++) {
        for (var col_index = 0; col_index < 9; col_index++) {
            if (col_index % 3 == 0 && col_index != 0) {
                process.stdout.write("| ");
            }
            process.stdout.write(matrix[row_index][col_index] + " ");
        }
        console.log("");
        if ((row_index + 1) % 3 == 0 && row_index != 0 && row_index != 8) {
            console.log("---------------------");
        }
    }
    console.log("");
}
function get_empty_cell(matrix) {
    for (var row_index = 0; row_index < 9; row_index++) {
        for (var col_index = 0; col_index < 9; col_index++) {
            if (matrix[row_index][col_index] == 0) {
                return [row_index, col_index];
            }
        }
    }
    return [-1, -1];
}

function get_possible_values(matrix, row, col) {
    var values = [1, 2, 3, 4, 5, 6, 7, 8, 9];
    for (var col_index = 0; col_index < 9; col_index++) {
        let element = matrix[row][col_index];
        values = values.filter(function (item) {
            return item !== element
        });
    }

    for (var row_index = 0; row_index < 9; row_index++) {
        let element = matrix[row_index][col];
        values = values.filter(function (item) {
            return item !== element
        });
    }

    block_no = (Math.floor(row / 3) * 3) + Math.floor(col / 3);
    block_row_index = Math.floor(block_no / 3) * 3;
    block_col_index = (block_no % 3) * 3;
    for (var row_index = block_row_index; row_index < block_row_index + 3; row_index++) {
        for (var col_index = block_col_index; col_index < block_col_index + 3; col_index++) {
            let element = matrix[row_index][col_index];
            values = values.filter(function (item) {
                return item !== element
            });
        }
    }

    return values;
}
function solve_sudoku(matrix) {
    const [row, col] = get_empty_cell(matrix);
    if (row == -1) {
        return matrix;
    }
    let possible_values = get_possible_values(matrix, row, col);
    for (var i = 0; i < possible_values.length; i++) {
        matrix[row][col] = possible_values[i];
        solution = solve_sudoku(matrix);
        if (solution != null) {
            return solution;
        }
        matrix[row][col] = 0;
    }
    return null;
}

function parseHrtimeToSeconds(hrtime) {
    var seconds = (hrtime[0] + (hrtime[1] / 1e9));
    return seconds;
}

function main() {
    const games_folder = "../games/";
    fs.readdir(games_folder, function (err, files) {
        if (err) {
            console.error("Could not list the directory.", err);
            process.exit(1);
        }
        // let solving_times = []
        let min_time = Number.MAX_VALUE;
        let max_time = Number.MIN_VALUE;
        let sum = 0;
        files.forEach((file, index) => {
            game_file = path.join(games_folder, file);
            var matrix = read_sudoku(game_file);
            var startTime = process.hrtime();
            solved = solve_sudoku(matrix);
            var elapsedSeconds = parseHrtimeToSeconds(process.hrtime(startTime));
            assert(solved != null);
            if (elapsedSeconds < min_time) {
                min_time = elapsedSeconds;
            }
            if (elapsedSeconds > max_time) {
                max_time = elapsedSeconds;
            }
            sum += elapsedSeconds;
        });
        let avg_time = Number.parseFloat(sum / files.length).toExponential()
        min_time = Number.parseFloat(min_time).toExponential();
        max_time = Number.parseFloat(max_time).toExponential();
        console.log(`Execution Times, min: ${min_time}s, avg: ${avg_time}s, max: ${max_time}s`);
    });
}

main()