#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int **sudoku_board;
int *worker_validation;

int **read_board_from_file(char *filename)
{
    FILE *fp = fopen(filename, "r");
    int **board = (int**)malloc(ROW_SIZE * sizeof(int*));
    for(int i= 0;i<ROW_SIZE;i++) {
        board[i] =(int*) malloc(COL_SIZE*sizeof(int));
    }
    
    if (fp == NULL){
        fprintf(stderr, "No file is found");
    }
    else {
        int row = 0;

        while (!feof(fp)) {

            for (int i = 0; i < ROW_SIZE; i++){

                fscanf(fp, "%d", &board[row][i]);
            }

            row++;
        }
        fclose(fp); 
    }

    return board;
}

int main(int argc, char *argv[]){
    read_board_from_file(argv[1]);
}

int is_board_valid()
{
    pthread_t *tid; /* the thread identifiers */
    pthread_attr_t attr;
    param_struct *parameter;

    // replace this comment with your code
}
