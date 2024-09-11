Sudoku Solver Written in Multiple Programming Languages
===========================
## What Is This?
------------
This is a simple sudoku solver code written in multiple languages to show the speed and performance of each language and how easy (or hard) it is to write the solution in multiple languages. The algorithm used in the solution is brute-force (i.e., trying all possible values per cell and backtracking if chosen value does not solve the game). 

## What are the programming languages used in this project?
------------
There are six(6) programming languages used in this project and they are: C, C++, Java, Javascript, Python, and Rust. 

## How to run this project?
------------
Each programming language has its own project folder. 

### C
------------
To run the code written in C, run the following commands. 
```
cd c
make
```

### C++
------------
To run the code written in C++, run the following commands. 
```
cd cpp
make
```

### Ncurses Game
------------
To run the code written in Ncurses API and C, run the following commands. 
```
cd ncurses_c
make
```

### Java
------------
To run the code written in Java, run the following commands. Please note that Maven is required and the project uses Java 21. 
```
cd java/sudoku_solver
mvn package
mvn exec:java
```

### Javascript
------------
To run the code written in Javascript, run the following commands. Please note that npm is required to install dependencies. 
```
cd javascript
npm install
node index.js
```

### Python
------------
To run the code written in Python, run the following commands. 
```
cd python
python solve_sudoku.py
```

### Rust
------------
To run the code written in Rust, run the following commands. Please note that Cargo is required to build and run the code.
```
cd rust
cargo run --release
```