#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

#define LENGTH 128     // length of the input
#define SHM_SIZE 5000  // size of shared memory

typedef struct {
    char firstMatrix[LENGTH];
    char secondMatrix[LENGTH];
    char operation[LENGTH];
    int type;
    int numOfOperations;
} SharedData;

int main() {
    key_t key ;
    if ((key = ftok("/tmp", 'A')) == -1){
        perror("ftok() failed") ;
        exit(EXIT_FAILURE) ;
    }

    int shmid = shmget(key, SHM_SIZE, 0600|IPC_CREAT|IPC_EXCL); // create shared memory
    if(shmid == -1){
        perror("shmid() failed");
        exit(EXIT_FAILURE);
    }

    SharedData *shared_data = (SharedData*) shmat(shmid, NULL, 0); // attach to shared memory
    if (shared_data == (SharedData *) -1){
        perror( "shmat failed" ) ;
        exit( EXIT_FAILURE ) ;
    }

    sem_t *sem_writer = sem_open("/sem_writer", O_CREAT, 0644, 1);
    sem_t *sem_reader = sem_open("/sem_reader", O_CREAT, 0644, 0);

    if (sem_writer == SEM_FAILED || sem_reader == SEM_FAILED) {
        perror("sem_open() failed");
        exit(EXIT_FAILURE);
    }

    sem_wait(sem_writer); // wait for writer semaphore

    int structCount = 0;
    while(1) {

        if(shared_data[0].type == 0)
            structCount = 0;

        fgets(shared_data[structCount].firstMatrix, LENGTH, stdin);
        if (strcmp(shared_data[structCount].firstMatrix, "END\n") == 0){
           // sem_post(sem_reader); // signal reader semaphore
            break;}
        // Determine the type of matrix
        if (strchr(shared_data[structCount].firstMatrix, 'i') != NULL) {
            shared_data[structCount].type = 3;
        } else if (strchr(shared_data[structCount].firstMatrix, '.') != NULL) {
            shared_data[structCount].type = 2;
        } else {
            shared_data[structCount].type = 1;
        }

        fgets(shared_data[structCount].secondMatrix, LENGTH, stdin);
        if (strcmp(shared_data[structCount].secondMatrix, "END\n") == 0) {
          //  sem_post(sem_reader); // signal reader semaphore;
            break;
        }
        if (strcmp(shared_data[structCount].secondMatrix, "NOT\n") == 0 || strcmp(shared_data[structCount].secondMatrix, "TRANSPOSE\n") == 0) {
            strcpy(shared_data[structCount].operation, shared_data[structCount].secondMatrix);
            strcpy(shared_data[structCount].secondMatrix, "");  // Clear secondMatrix for single matrix operations
        } else {
            fgets(shared_data[structCount].operation, LENGTH, stdin);
            if (strcmp(shared_data[structCount].operation, "END\n") == 0) {
              //  sem_post(sem_reader); // signal reader semaphore;
                break;
            }
        }
        structCount++;
        for (int i =0;i<structCount;i++){
            shared_data[i].numOfOperations = structCount;
        }
        sem_post(sem_reader); // signal reader semaphore
        sem_post(sem_writer);
    }


    shmdt(shared_data); // detach from shared memory
    if (shmctl(shmid,IPC_RMID, NULL) == -1){
        perror( "shmctl failed" ) ;
        exit( EXIT_FAILURE ) ;
    }
    sem_close(sem_writer);
    sem_close(sem_reader);
    sem_unlink("/sem_writer");
    sem_unlink("/sem_reader");
    return 0;
}
