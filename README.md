# Matrix-calculator
Authored by Mohamad Dweik

==Description==
This program performs various matrix operations including addition, subtraction, multiplication, logical AND, logical OR, NOT, and transpose. It supports matrices of different types: integer, double, and complex. The program reads input matrices and operations from the user and processes them accordingly. It can handle operations using shared memory or threads.

==Program Database==
The program only uses structs that stores the inputs and then send it to the shared memory ot to the thread to do the calculations.

==Functions==
The program includes the following key functions:

1.roundToNearestTenth: Rounds a double number to the nearest tenth.
2.stringToMatrix: Converts a string representation of an int matrix to an actual int matrix.
3.stringToDoubleMatrix: Converts a string representation of a double matrix to an actual double matrix.
4.stringToComplexMatrix: Converts a string representation of a complex matrix to an actual complex matrix.
5.addMatrices: Adds two matrices of the same type.
6.subMatrices: Subtracts one matrix from another of the same type.
7.multiplyMatrices: Multiplies two matrices of the same type.
8.andMatrices: Performs a bitwise AND operation between two int matrices.
9.orMatrices: Performs a bitwise OR operation between two int matrices.
10.notMatrix: Performs a bitwise NOT operation on an int matrix.
11.transposeMatrix: Transposes a matrix of any type.
12.checkBinary: Checks if a matrix is binary (contains only 0s and 1s).
13.printMatrix: Prints a matrix in a specific format based on its type.
14.freeMatrix: Frees the memory allocated for a matrix.
15.combineResults: combine the results in a one final result matrix.
16.threadFunction: handle the threads

==Program Files==
main programs:
shm_writer.c
shm_reader.c
threads_calc.c
compile and run:
run_me_reader.sh
run_me_writer.sh
run_me_threads.sh

==how to compile==
./run_me_writer.sh
./run_me_reader.sh
./run_me_threads.sh

==input==
Matrices and operations entered in the format (rows,cols:elements), followed by operation commands like ADD, SUB, MUL, AND, OR, NOT, TRANSPOSE, or END.

==output==
Result of matrix operations, or ERR for errors.

