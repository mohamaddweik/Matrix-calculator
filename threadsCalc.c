#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <complex.h>

#define LENGTH 128  // length of the input

typedef struct {
    void* matrix1;
    void* matrix2;
    void* resultMatrix;
    int rows;
    int cols;
    int type;
    char operation[LENGTH];
} ThreadData;

// function that rounds the double number
double roundToNearestTenth(double number) {
    return ((int)(number * 10 + 0.5)) / 10.0;
}

// function that checks if a matrix is binary
int checkBinary(int** matrix, int rows , int cols){
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if(matrix[i][j] != 0 && matrix[i][j] != 1)
                return -1;
        }
    }
    return 1;
}

// function that transforms a string to int matrix
int** stringToMatrix(const char* str, int* rows, int* cols) {
    char input[LENGTH];
    strcpy(input, str);

    char* colonPos = strchr(input, ':');

    *colonPos = '\0';
    char* dims = input + 1;
    char* elements = colonPos + 1;

    sscanf(dims, "%d,%d", rows, cols);

    int** matrix = malloc(*rows * sizeof(int*));
    for (int i = 0; i < *rows; ++i) {
        matrix[i] = malloc(*cols * sizeof(int));
    }

    char* token = strtok(elements, ",");
    int i = 0, j = 0;
    while (token != NULL) {
        matrix[i][j] = atoi(token);
        j++;
        if (j == *cols) {
            j = 0;
            i++;
        }
        token = strtok(NULL, ",");
    }

    return matrix;
}

// function that transforms a string to double matrix
double** stringToDoubleMatrix(const char* str, int* rows, int* cols) {
    char input[LENGTH];
    strcpy(input, str);

    char* colonPos = strchr(input, ':');
    *colonPos = '\0';
    char* dims = input + 1;
    char* elements = colonPos + 1;

    sscanf(dims, "%d,%d", rows, cols);

    double** matrix = malloc(*rows * sizeof(double*));
    for (int i = 0; i < *rows; ++i) {
        matrix[i] = malloc(*cols * sizeof(double));
    }

    char* token = strtok(elements, ",");
    int i = 0, j = 0;
    while (token != NULL) {
        matrix[i][j] = roundToNearestTenth(atof(token));
        j++;
        if (j == *cols) {
            j = 0;
            i++;
        }
        token = strtok(NULL, ",");
    }

    return matrix;
}

// function that transforms a string to complex matrix
int complex** stringToComplexMatrix(const char* str, int* rows, int* cols) {
    char input[LENGTH];
    strcpy(input, str);

    char* colonPos = strchr(input, ':');
    *colonPos = '\0';
    char* dims = input + 1;
    char* elements = colonPos + 1;

    sscanf(dims, "%d,%d", rows, cols);

    int complex** matrix = malloc(*rows * sizeof(int complex*));
    for (int i = 0; i < *rows; ++i) {
        matrix[i] = malloc(*cols * sizeof(int complex));
    }

    char* token = strtok(elements, ",");
    int i = 0, j = 0;
    while (token != NULL) {
        int real, imag;
        if (sscanf(token, "%d%di", &real, &imag) != 2) {
            imag = real;
            real = 0;
        }
        matrix[i][j] = real + imag * I;
        j++;
        if (j == *cols) {
            j = 0;
            i++;
        }
        token = strtok(NULL, ",");
    }

    return matrix;
}

// function that adds two matrices
void* addMatrices(void* matrix1, void* matrix2, int rows, int cols, int type) {
    void** resultMatrix = malloc(rows * sizeof(void*));

    switch (type) {
        case 1: // int
            for (int i = 0; i < rows; ++i) {
                ((int**)resultMatrix)[i] = malloc(cols * sizeof(int));
                for (int j = 0; j < cols; ++j) {
                    ((int**)resultMatrix)[i][j] = ((int**)matrix1)[i][j] + ((int**)matrix2)[i][j];
                }
            }
            break;
        case 2: // double
            for (int i = 0; i < rows; ++i) {
                ((double**)resultMatrix)[i] = malloc(cols * sizeof(double));
                for (int j = 0; j < cols; ++j) {
                    ((double**)resultMatrix)[i][j] = ((double**)matrix1)[i][j] + ((double**)matrix2)[i][j];
                }
            }
            break;
        case 3: // double complex
            for (int i = 0; i < rows; ++i) {
                ((int complex**)resultMatrix)[i] = malloc(cols * sizeof(int complex));
                for (int j = 0; j < cols; ++j) {
                    ((int complex**)resultMatrix)[i][j] = ((int complex**)matrix1)[i][j] + ((int complex**)matrix2)[i][j];
                }
            }
            break;
    }
    return resultMatrix;
}

// function that subtracts two matrices
void* subMatrices(void* matrix1, void* matrix2, int rows, int cols, int type) {
    void** resultMatrix = malloc(rows * sizeof(void*));

    switch (type) {
        case 1: // int
            for (int i = 0; i < rows; ++i) {
                ((int**)resultMatrix)[i] = malloc(cols * sizeof(int));
                for (int j = 0; j < cols; ++j) {
                    ((int**)resultMatrix)[i][j] = ((int**)matrix1)[i][j] - ((int**)matrix2)[i][j];
                }
            }
            break;
        case 2: // double
            for (int i = 0; i < rows; ++i) {
                ((double**)resultMatrix)[i] = malloc(cols * sizeof(double));
                for (int j = 0; j < cols; ++j) {
                    ((double**)resultMatrix)[i][j] = ((double**)matrix1)[i][j] - ((double**)matrix2)[i][j];
                }
            }
            break;
        case 3: // double complex
            for (int i = 0; i < rows; ++i) {
                ((int complex**)resultMatrix)[i] = malloc(cols * sizeof(int complex));
                for (int j = 0; j < cols; ++j) {
                    ((int complex**)resultMatrix)[i][j] = ((int complex**)matrix1)[i][j] - ((int complex**)matrix2)[i][j];
                }
            }
            break;
    }
    return resultMatrix;
}

// function that multiply two matrices
void* multiplyMatrices(void* matrix1, void* matrix2, int rows1, int cols1, int type) {
    void** resultMatrix = (void*)malloc(rows1 * sizeof(void*));

    switch (type) {
        case 1: // int
            for (int i = 0; i < rows1; ++i) {
                resultMatrix[i] = (int*)malloc(cols1 * sizeof(int));
                for (int j = 0; j < cols1; ++j) {
                    ((int**)resultMatrix)[i][j] = 0;
                    for (int k = 0; k < cols1; ++k) {
                        ((int**)resultMatrix)[i][j] += ((int**)matrix1)[i][k] * ((int**)matrix2)[k][j];
                    }
                }
            }
            break;
        case 2: // double
            for (int i = 0; i < rows1; ++i) {
                resultMatrix[i] = (double*)malloc(cols1 * sizeof(double));
                for (int j = 0; j < cols1; ++j) {
                    ((double**)resultMatrix)[i][j] = 0.0;
                    for (int k = 0; k < cols1; ++k) {
                        roundToNearestTenth((((double**)resultMatrix)[i][j]) += ((double**)matrix1)[i][k] * ((double**)matrix2)[k][j]);
                    }
                }
            }
            break;
        case 3: // double complex
            for (int i = 0; i < rows1; ++i) {
                resultMatrix[i] = (double complex*)malloc(cols1 * sizeof(int complex));
                for (int j = 0; j < cols1; ++j) {
                    ((int complex**)resultMatrix)[i][j] = 0.0 + 0.0 * I;
                    for (int k = 0; k < cols1; ++k) {
                        ((int complex**)resultMatrix)[i][j] += ((int complex**)matrix1)[i][k] * ((int complex**)matrix2)[k][j];
                    }
                }
            }
            break;
    }

    return resultMatrix;
}

// function that do AND between two matrices
int** andMatrices(int** matrix1, int** matrix2, int rows, int cols){
    int** resultMatrix = (int**)malloc(rows * sizeof(int*));

    for (int i = 0; i < rows; ++i) {
        resultMatrix[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; ++j) {
            resultMatrix[i][j] = matrix1[i][j] & matrix2[i][j];
        }
    }
    return resultMatrix;
}

// function that do OR between two matrices
int** orMatrices(int** matrix1, int** matrix2, int rows, int cols){
    int** resultMatrix = (int**)malloc(rows * sizeof(int*));

    for (int i = 0; i < rows; ++i) {
        resultMatrix[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; ++j) {
            resultMatrix[i][j] = matrix1[i][j] | matrix2[i][j];
        }
    }
    return resultMatrix;
}

// a thread function
void* threadFunction(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    if (strcmp(data->operation, "ADD\n") == 0) {
        data->resultMatrix = addMatrices(data->matrix1, data->matrix2, data->rows, data->cols, data->type);
    } else if (strcmp(data->operation, "SUB\n") == 0) {
        data->resultMatrix = subMatrices(data->matrix1, data->matrix2, data->rows, data->cols, data->type);
    } else if (strcmp(data->operation, "MUL\n") == 0) {
        data->resultMatrix = multiplyMatrices(data->matrix1, data->matrix2, data->rows, data->cols, data->type);
    } else if (strcmp(data->operation, "AND\n") == 0) {
        data->resultMatrix = andMatrices(data->matrix1, data->matrix2, data->rows, data->cols);
    } else if (strcmp(data->operation, "OR\n") == 0) {
        data->resultMatrix = orMatrices(data->matrix1, data->matrix2, data->rows, data->cols);
    }
    pthread_exit(&(data->resultMatrix));
}

// function that prints a matrix
void printMatrix(void* matrix, int rows, int cols, int type) {
    switch (type) {
        case 1: // int
            printf("(%d,%d:", rows, cols);
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    printf("%d", ((int**)matrix)[i][j]);
                    if (j < cols - 1) printf(",");
                }
                if (i < rows - 1) printf(",");
            }
            printf(")\n");
            break;
        case 2: // double
            printf("(%d,%d:", rows, cols);
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    printf("%.1lf", roundToNearestTenth(((double**)matrix)[i][j]));
                    if (j < cols - 1) printf(",");
                }
                if (i < rows - 1) printf(",");
            }
            printf(")\n");
            break;
        case 3: // double complex
            printf("(%d,%d:", rows, cols);
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if ((int)creal(((int complex**)matrix)[i][j]) == 0) printf("%di", (int)cimag(((int complex**)matrix)[i][j]));
                    else printf("%d%+di", (int)creal(((int complex**)matrix)[i][j]), (int)cimag(((int complex**)matrix)[i][j]));
                    if (j < cols - 1) printf(",");
                }
                if (i < rows - 1) printf(",");
            }
            printf(")\n");
            break;
    }
}

// function that combine all the results into one final result
void combineResults(void** results, int rows, int cols, int type, int numResults, char* operation) {
    void* finalResult = results[0];
    for (int i = 1; i < numResults ; ++i) {
        if (strcmp(operation, "ADD\n") == 0) {
            finalResult = addMatrices(finalResult, results[i], rows, cols, type);
        } else if (strcmp(operation, "SUB\n") == 0) {
            finalResult = subMatrices(finalResult, results[i], rows, cols, type);
        } else if (strcmp(operation, "MUL\n") == 0) {
            finalResult = multiplyMatrices(finalResult, results[i], rows, cols, type);
        } else if (strcmp(operation, "AND\n") == 0) {
            finalResult = andMatrices(finalResult, results[i], rows, cols);
        } else if (strcmp(operation, "OR\n") == 0) {
            finalResult = orMatrices(finalResult, results[i], rows, cols);
        }
    }
    printMatrix(finalResult,rows,cols,type);
}

// function that frees a matrix
void freeMatrix(void* matrix, int rows, int type) {
    if (type == 1) {
        int** intMatrix = (int**)matrix;
        for (int i = 0; i < rows; ++i) {
            free(intMatrix[i]);
        }
        free(intMatrix);
    } else if (type == 2) {
        double** doubleMatrix = (double**)matrix;
        for (int i = 0; i < rows; ++i) {
            free(doubleMatrix[i]);
        }
        free(doubleMatrix);
    } else if (type == 3) {
        double complex** complexMatrix = (double complex**)matrix;
        for (int i = 0; i < rows; ++i) {
            free(complexMatrix[i]);
        }
        free(complexMatrix);
    }
}

// function that checks if a number is power of two
int isPowerOfTwo(int n) {
    return (n & (n - 1)) == 0 && n != 0;
}

int main() {
        char input[LENGTH];
        while (1) {
            int flag =0;
            fgets(input, LENGTH, stdin);
            if (strcmp(input, "END\n") == 0) break;

            // Read matrices until an operation is found
            int numMatrices = 0;
            void *matrices[100]; // Assuming a maximum of 100 matrices for simplicity
            int rows, cols, type;

            while (1) {
                if (input[0] != '(') break; // Check if it's an operation

                if (strchr(input, 'i') != NULL) {
                    matrices[numMatrices] = stringToComplexMatrix(input, &rows, &cols);
                    type = 3;
                } else if (strchr(input, '.') != NULL) {
                    matrices[numMatrices] = stringToDoubleMatrix(input, &rows, &cols);
                    type = 2;
                } else {
                    matrices[numMatrices] = stringToMatrix(input, &rows, &cols);
                    type = 1;
                }
                numMatrices++;
                fgets(input, LENGTH, stdin);
                if (strcmp(input, "END\n") == 0) return 0;
            }

            // The last read input is the operation
            char operation[LENGTH];
            strcpy(operation, input);

            if(strcmp(operation,"AND\n") == 0 || strcmp(operation,"OR\n") == 0){
                for (int i=0 ; i < numMatrices ; i++){
                    if(checkBinary(matrices[numMatrices],rows,cols) == -1)
                        flag = 1;
                }
            }

            if(flag == 1){
                printf("ERR\n");
                continue;
            }

            if (isPowerOfTwo(numMatrices)) {
                while (numMatrices > 1) {
                    int newNumMatrices = numMatrices / 2;
                    pthread_t threads[newNumMatrices];
                    ThreadData data[newNumMatrices];
                    void *results[newNumMatrices];

                    for (int i = 0; i < newNumMatrices; ++i) {
                        data[i].matrix1 = matrices[2 * i];
                        data[i].matrix2 = matrices[2 * i + 1];
                        data[i].rows = rows;
                        data[i].cols = cols;
                        data[i].type = type;
                        strcpy(data[i].operation, operation);
                        pthread_create(&threads[i], NULL, threadFunction, &data[i]);
                    }

                    for (int i = 0; i < newNumMatrices; ++i) {
                        pthread_join(threads[i], (void **) &results[i]);
                        results[i] = data[i].resultMatrix;
                    }

                    for (int i = 0; i < newNumMatrices * 2; ++i) {
                        freeMatrix(matrices[i], rows, type);
                    }

                    for (int i = 0; i < newNumMatrices; ++i) {
                        if (type == 1)
                            matrices[i] = malloc(cols * sizeof(int));
                        else if (type == 2)
                            matrices[i] = malloc(cols * sizeof(double));
                        else
                            matrices[i] = malloc(cols * sizeof(int complex));

                        matrices[i] = results[i];
                    }

                    numMatrices = newNumMatrices;
                }

                // Print the final result
                printMatrix(matrices[0], rows, cols, type);
                freeMatrix(matrices[0], rows, type);

            } else {
                pthread_t threads[numMatrices / 2];
                ThreadData data[numMatrices / 2];
                void *results[numMatrices / 2];

                for (int i = 0; i < numMatrices / 2; ++i) {
                    data[i].matrix1 = matrices[2 * i];
                    data[i].matrix2 = matrices[2 * i + 1];
                    data[i].rows = rows;
                    data[i].cols = cols;
                    data[i].type = type;
                    strcpy(data[i].operation, operation);
                    pthread_create(&threads[i], NULL, threadFunction, &data[i]);
                }

                for (int i = 0; i < numMatrices / 2; ++i) {
                    pthread_join(threads[i], (void **) &results[i]);
                    results[i] = data[i].resultMatrix;
                }

                combineResults(results, rows, cols, type, numMatrices / 2, operation);

                for (int i = 0; i < numMatrices; ++i) {
                    freeMatrix(matrices[i], rows, type);
                }
                for (int i = 0; i < numMatrices / 2; ++i) {
                    freeMatrix(results[i], rows, type);
                }
            }
        }

        return 0;
    }

