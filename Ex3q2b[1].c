#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>


#define LENGTH 128     // length of the input string
#define SHM_SIZE 5000  // size of shared memory

// function that rounds the double number
double roundToNearestTenth(double number) {
    return ((int)(number * 10 + 0.5)) / 10.0;
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

// function that do OR between two matrice
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

// function that return the opposite of a binary matrix
int** notMatrix(int** matrix , int rows, int cols){
    int** resultMatrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; ++i) {
        resultMatrix[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; ++j) {
            resultMatrix[i][j] = !matrix[i][j];
        }
    }
    return resultMatrix;
}

// function that transpose a matrix
void* transposeMatrix(void* matrix, int rows, int type){
    void** resultMatrix = (void*)malloc(rows * sizeof(void*));
    switch (type) {
        case 1: // int
            for (int i = 0; i < rows; ++i) {
                ((int**)resultMatrix)[i] = malloc(rows * sizeof(int));
                for (int j = 0; j < rows; ++j) {
                    ((int**)resultMatrix)[i][j] = ((int**)matrix)[j][i];
                }
            }
            break;
        case 2: // double
            for (int i = 0; i < rows; ++i) {
                ((double**)resultMatrix)[i] = malloc(rows * sizeof(double));
                for (int j = 0; j < rows; ++j) {
                    ((double**)resultMatrix)[i][j] = ((double**)matrix)[j][i];
                }
            }
            break;
        case 3: // double complex
            for (int i = 0; i < rows; ++i) {
                ((int complex**)resultMatrix)[i] = malloc(rows * sizeof(int complex));
                for (int j = 0; j < rows; ++j) {
                    ((int complex**)resultMatrix)[i][j] = ((int complex**)matrix)[j][i];
                }
            }
            break;
    }
    return resultMatrix;

}

// function that checks if matrix is binary
int checkBinary(int** matrix, int rows , int cols){
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if(matrix[i][j] != 0 && matrix[i][j] != 1)
                return -1;
        }
    }
    return 1;
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

typedef struct {
    char firstMatrix[LENGTH];
    char secondMatrix[LENGTH];
    char operation[LENGTH];
    int type;
    int numOfOperation;
} SharedData;

int main() {
    key_t key;
    if ((key = ftok("/tmp", 'A')) == -1){
        perror("ftok() failed");
        exit(EXIT_FAILURE);
    }

    int shmid = shmget(key, 0, 0600); // create shared memory
    if (shmid == -1){
        perror("shmget() failed");
        exit(EXIT_FAILURE);
    }

    SharedData *shared_data = (SharedData*) shmat(shmid, NULL, 0); // attach to shared memory
    if (shared_data == (SharedData *) -1){
        perror("shmat() failed");
        exit(EXIT_FAILURE);
    }

    sem_t *sem_writer = sem_open("/sem_writer", 0);
    sem_t *sem_reader = sem_open("/sem_reader", 0);

    if (sem_writer == SEM_FAILED || sem_reader == SEM_FAILED) {
        perror("sem_open() failed");
        exit(EXIT_FAILURE);
    }
    sem_wait(sem_reader); // wait for reader semaphore

    for(int i = 0 ; i <shared_data[0].numOfOperation;i++) {

        void *matrix1 = NULL;
        void *matrix2 = NULL;
        void *resultMatrix = NULL;
        int rows1, cols1, rows2, cols2;

        if (strcmp(shared_data[i].firstMatrix, "END\n") == 0 ||
            strcmp(shared_data[i].secondMatrix, "END\n") == 0 ||
            strcmp(shared_data[i].operation, "END\n") == 0) {
            shmdt(shared_data); // detach from shared memory
            sem_close(sem_writer);
            sem_close(sem_reader);
            exit(EXIT_SUCCESS);
        }

        if (shared_data[i].type == 3) {
            matrix1 = stringToComplexMatrix(shared_data[i].firstMatrix, &rows1, &cols1);
        } else if (shared_data[i].type == 2) {
            matrix1 = stringToDoubleMatrix(shared_data[i].firstMatrix, &rows1, &cols1);
        } else if (shared_data[i].type == 1){
            matrix1 = stringToMatrix(shared_data[i].firstMatrix, &rows1, &cols1);
        }

        // Check if it's a single matrix operation
        if (strcmp(shared_data[i].secondMatrix, "") != 0) {
            if (shared_data[i].type == 3) {
                matrix2 = stringToComplexMatrix(shared_data[i].secondMatrix, &rows2, &cols2);
            } else if (shared_data[i].type == 2) {
                matrix2 = stringToDoubleMatrix(shared_data[i].secondMatrix, &rows2, &cols2);
            } else {
                matrix2 = stringToMatrix(shared_data[i].secondMatrix, &rows2, &cols2);
            }
        }

        if (strcmp(shared_data[i].operation, "ADD\n") == 0) {
            resultMatrix = addMatrices(matrix1, matrix2, rows1, cols1, shared_data[i].type);
        } else if (strcmp(shared_data[i].operation, "SUB\n") == 0) {
            resultMatrix = subMatrices(matrix1, matrix2, rows1, cols1, shared_data[i].type);
        } else if (strcmp(shared_data[i].operation, "MUL\n") == 0) {
            resultMatrix = multiplyMatrices(matrix1, matrix2, rows1, cols1, shared_data[i].type);
        } else if (strcmp(shared_data[i].operation, "AND\n") == 0) {
            if (shared_data[i].type == 1 && checkBinary(matrix1, rows1, cols1) == 1 &&
                checkBinary(matrix2, rows2, cols2) == 1) {
                resultMatrix = (void **) andMatrices(matrix1, matrix2, rows1, cols1);
            } else printf("ERR\n");

        } else if (strcmp(shared_data[i].operation, "OR\n") == 0) {
            if (shared_data[i].type == 1 && checkBinary(matrix1, rows1, cols1) == 1 &&
                checkBinary(matrix2, rows2, cols2) == 1) {
                resultMatrix = (void **) orMatrices(matrix1, matrix2, rows1, cols1);
            } else printf("ERR\n");

        } else if (strcmp(shared_data[i].operation, "TRANSPOSE\n") == 0) {
            resultMatrix = transposeMatrix(matrix1, rows1, shared_data[i].type);

        } else if (strcmp(shared_data[i].operation, "NOT\n") == 0) {
            if (shared_data[i].type == 1 && checkBinary(matrix1, rows1, cols1) == 1) {
                resultMatrix = notMatrix((int **) matrix1, rows1, cols1);
            } else printf("ERR\n");
        }

        if (resultMatrix != NULL) {
            printMatrix(resultMatrix, rows1, cols1, shared_data[i].type);
            freeMatrix(resultMatrix, rows1, shared_data[i].type);
        }

        freeMatrix(matrix1, rows1, shared_data[i].type);
        if (matrix2){ freeMatrix(matrix2, rows2, shared_data[i].type);}
    }
    memset(shared_data, 0, SHM_SIZE); //set tha shared memory to 0's

    shmdt(shared_data); // detach from shared memory
    sem_close(sem_writer);
    sem_close(sem_reader);
    return 0;
}
