from time import time
import os
def read_sudoku(filename: str):
    matrix = []
    with open(filename, "r") as f:
        for line in f.readlines():
            if len(line) == 0:
                continue
            row = [*line]
            if row[-1] == "\n":
                del row[-1]
            numbers = [int(item) for item in row]
            matrix.append(numbers)
    return matrix

def print_matrix(title: str, matrix: list):
    if matrix == None:
        return
    print(title)
    for rowIndex in range(len(matrix)):
        for colIndex in range(len(matrix[rowIndex])):
            if colIndex % 3 == 0 and colIndex != 0:
                print("| ", end="")
            print(f"{matrix[rowIndex][colIndex]} ", end="")
        print()
        if (rowIndex + 1) % 3 == 0 and rowIndex != 0 and rowIndex != 8:
            print("-"*21)
    print()

def get_empty_cell(matrix: list):
    for rowIndex in range(len(matrix)):
        for colIndex in range(len(matrix[rowIndex])):
            if matrix[rowIndex][colIndex] == 0:
                return rowIndex, colIndex
    return -1, -1

def get_possible_values(matrix: list, row: int, col: int):
    values = [1, 2, 3, 4, 5, 6, 7, 8, 9]
    currentRow = matrix[row]
    # check the values of the current row
    for element in currentRow:
        if element in values: 
            values.remove(element)
    # check the values of the current column
    for rowIndex in range(len(matrix)):
        element = matrix[rowIndex][col]
        if element in values:
            values.remove(element)
    # print(f"{row=}, {col=}")
    # check the values of the current block
    block_no = ((row // 3) * 3) + (col // 3)
    block_row_index = (block_no // 3) * 3
    block_col_index = (block_no % 3) * 3
    # print(f"{block_no=}, {block_row_index=}, {block_col_index=}")
    for rowIndex in range(block_row_index, block_row_index+3):
        for colIndex in range(block_col_index, block_col_index+3):
            # print(f"{rowIndex=}, {colIndex=}")
            element = matrix[rowIndex][colIndex]
            if element in values:
                values.remove(element)
    return values

def solve_sudoku(matrix: list):
    row, col = get_empty_cell(matrix)
    if row == -1:
        # base case, no more empty cells
        return matrix
    possible_values = get_possible_values(matrix, row, col)
    # print(f"row: {row}, col: {col}, values: {possible_values}")
    if len(possible_values) == 0:
        # solution does not exist for this matrix, backtrack
        return None
    for value in possible_values:
        matrix[row][col] = value
        # print(f"Trying {value} for row: {row}, col: {col}")
        solution = solve_sudoku(matrix)
        if solution != None: 
            return solution
        matrix[row][col] = 0
    return None



def main():
    solving_times = []
    GAMES_FOLDER = "../games/"
    game_files = [os.path.join(GAMES_FOLDER, file) for file in os.listdir(GAMES_FOLDER) if os.path.isfile(os.path.join(GAMES_FOLDER, file))]
    print(f"There are {len(game_files)} games to solve")
    for game_file in game_files:
        # sudoku_matrix = read_sudoku(game_file)
        sudoku_matrix = read_sudoku("../games/game00.txt")
        # print_matrix("Before Solving", sudoku_matrix)
        start = time()
        solved = solve_sudoku(sudoku_matrix)
        end = time()
        assert(solved != None)
        solving_times.append(end-start)
        # print_matrix("After Solving", solved)
    minTime = min(solving_times)
    avgTime = sum(solving_times)/len(solving_times)
    maxTime = max(solving_times)
    print(f"Execution Times, min: {minTime:e}s, avg: {avgTime:e}s, max: {maxTime:e}s")


if __name__=="__main__":
    main()    