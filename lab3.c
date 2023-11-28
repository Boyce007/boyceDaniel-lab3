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
        return 1;
    }
    
        int row = 0;
        
        for(int i = 0;i<ROW_SIZE;i++) {
            for (int j = 0;j<COL_SIZE;j++) {
                fscanf(fp,"%d,",&board[i][j]);
            }

        
        }
        fclose(fp);
    return board;
}





int is_board_valid() {
    pthread_t tid = (pthread_t*) malloc(sizeof(int)*NUM_OF_THREADS);
    pthread_attr_t attr;
    param_struct* params = (param_struct*)malloc(sizeof(param_struct)*NUM_OF_THREADS);
    

    for(int i= 0;i<ROW_SIZE;i++) {
        params[i].id = i;
        params[i].starting_row = i;
        params[i].starting_col = 0;
        params[i].ending_row = i;
        params[i].ending_col = COL_SIZE-1;
        pthread_create(&(tid[i]), &attr, validate, &(params[i]));
    }


    for(int i= ROW_SIZE;i<ROW_SIZE+COL_SIZE;i++) {
        params[i].starting_row = 0;
        params[i].starting_col = i-ROW_SIZE;
        params[i].ending_row = ROW_SIZE-1;
        params[i].ending_col = i-ROW_SIZE;
        pthread_create(&(tid[i]), &attr, validate, &(params[i]));
    }

    for(int i =ROW_SIZE+COL_SIZE;i< NUM_OF_THREADS;i++ ) {
        
        if((i - ROW_SIZE+COL_SIZE)+1 %3 ==0) {
            params[i].starting_row = i - ROW_SIZE+COL_SIZE;
            params[i].starting_col = (i+2) -ROW_SIZE+COL_SIZE ;
            params[i].ending_row = i - ROW_SIZE+COL_SIZE;
            params[i].ending_col = i+2 - ROW_SIZE+COL_SIZE;
            pthread_create(&(tid[i]), &attr, validate, &(params[i]));

        } else {
            continue;
        }
    }

    for(int i =0;i<NUM_OF_THREADS;i++) {
        pthread_join(tid[i],NULL);
        if(worker_validation[i]!=1) {
            return 0;
        }
    }
    return 1;
}

void* validate(void* p) {
    param_struct* param = (param_struct*) p;
    int validate_arr[9] = {0,0,0,0,0,0,0,0,0};
    for(int i =param->starting_row ;i<param->ending_row;i++) {
        for (int j = param->starting_col;i<param->ending_row;i++) {
            int current_row_val = sudoku_board[i][j];
            if(validate_arr[current_row_val-1]==1) { 
                worker_validation[param->id] = 0; 
                break;
            }
            validate_arr[current_row_val-1] = 1;
        }
    }

    worker_validation[param->id] = 1;
}





    


 








