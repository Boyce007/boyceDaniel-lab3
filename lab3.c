#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

// extern int **sudoku_board;
int** test_board;
int *worker_validation;

int **read_board_from_file(char *filename)
{
    FILE *fp = fopen(filename, "r");
    
    test_board = (int**)malloc(ROW_SIZE * sizeof(int*));

    for(int i= 0;i<ROW_SIZE;i++) {
        
        test_board[i] =(int*) malloc(COL_SIZE*sizeof(int));
    }
    if (fp == NULL){
        fprintf(stderr, "No file is found");
        exit(1);
    }
    for(int i = 0;i<ROW_SIZE;i++) {
        for (int j = 0;j<COL_SIZE;j++) {
            fscanf(fp,"%d,",&(test_board[i][j]));
        }
    }
        fclose(fp);

    return test_board;
}



void* validate(void* p) {
    printf("In validate ");
    param_struct* param = (param_struct*) p;
    int validate_arr[9] = {0,0,0,0,0,0,0,0,0};
    printf("ID:%d \n",param->id);
    if (param->id<0 || param->id>26) {
        return NULL;
    }
    for(int i =param->starting_row ;i<=param->ending_row;i++) {
        for (int j = param->starting_col;j<= param->ending_col;j++) {          
            int current_row_val = test_board[i][j];
            if(validate_arr[current_row_val-1]==1) { 
                worker_validation[param->id] = 0; 
                return NULL;
            } else {
                validate_arr[current_row_val-1] = 1;
            } 
        }
    }
    printf("param id %d ",param->id);
    worker_validation[param->id] = 1;
    printf("executed \n");
    pthread_exit(0);
}






int is_board_valid() {
    pthread_t* tid = (pthread_t*) malloc(sizeof(int)*NUM_OF_THREADS);
    pthread_attr_t attr;
    param_struct* params = (param_struct*)malloc(sizeof(param_struct)*NUM_OF_THREADS);
    worker_validation = (int*) malloc(sizeof(int)*NUM_OF_THREADS);
    int i =0;
    pthread_attr_init(&attr);


    for(int row= 0;row<ROW_SIZE;row++) {
            params[i].id = i;
            params[i].starting_row = row;
            params[i].starting_col = 0;
            params[i].ending_row = row;
            params[i].ending_col = COL_SIZE-1;           
            printf("i: %d params[i] %d\n",i,params[i].id );
            pthread_create(&(tid[i]), &attr, validate, &(params[i]));
            printf("after create i: %d params[i] %d\n",i,params[i].id );
            i++;
    }

    for(int col= 0; col<COL_SIZE; col++) {
        params[i].id = i;
        params[i].starting_row = 0;
        params[i].starting_col = col;
        params[i].ending_row = ROW_SIZE-1;
        params[i].ending_col = col;
        pthread_create(&(tid[i]), &attr, validate, &(params[i]));
        i++;
    }

    for(int row = 0; row<ROW_SIZE; row+=3) {
        for(int col =0; col<COL_SIZE; col+=3) {
                params[i].id = i;
                params[i].starting_row = row;
                params[i].starting_col = col;
                params[i].ending_row = row+2 ;
                params[i].ending_col = col +2;
                pthread_create(&(tid[i]), &attr, validate, &(params[i]));
                i++;
        }
        
    }


    for(int i =0;i<NUM_OF_THREADS;i++) { 
        pthread_join(tid[i],NULL);
        
    }
    for(int i =0;i<NUM_OF_THREADS;i++) {
        printf("id:%d value:%d \n",i,worker_validation[i]);
        // if(worker_validation[i]!=1) {
        //     return 0;
        // }
    }
        
    
    return 1;
}

int main(int argc, char *argv[]){
    
    int** board = read_board_from_file(argv[1]);
    
    int val = is_board_valid();
    // for(int i =0;i<ROW_SIZE;i++){
    //     for(int j =0;j<COL_SIZE;j++) {
    //         printf("%d,",board[i][j]);
    //     }
    //     printf("\n");

    // }    
    
    

}







    


 








