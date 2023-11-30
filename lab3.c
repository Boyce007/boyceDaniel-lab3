#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

// extern int **sudoku_board;
int** test_board;
int *worker_validation;

int **read_board_from_file(char *filename)
{
    // printf("Before open");
    FILE *fp = fopen(filename, "r");
    // printf("%s \n","after open");
    test_board = (int**)malloc(ROW_SIZE * sizeof(int*));

    for(int i= 0;i<ROW_SIZE;i++) {
        // printf("i:%d \n",i);
        test_board[i] =(int*) malloc(COL_SIZE*sizeof(int));
    }
    if (fp == NULL){
        fprintf(stderr, "No file is found");
        exit(1);
    }
    // printf("%s","before read");   
    for(int i = 0;i<ROW_SIZE;i++) {
        for (int j = 0;j<COL_SIZE;j++) {
            // printf("i:%d,j:%d",i,j);
            fscanf(fp,"%d,",&(test_board[i][j]));
            
        }
    }
        fclose(fp);

    return test_board;
}



void* validate(void* p) {
    
    param_struct* param = (param_struct*) p;
    
    int validate_arr[9] = {0,0,0,0,0,0,0,0,0};
    printf("in Validate ");
    printf("begrow: %d endRow%d begCol%d endCol%d \n",param->starting_row,param->ending_row,param->starting_col,param->ending_col);
    for(int i =param->starting_row ;i<param->ending_row;i++) {
        for (int j = param->starting_col;j<param->ending_col;j++) {
            int current_row_val = test_board[i][j];
            printf("test board val");
            printf("row:%d val:%d \n ",i,current_row_val);
            if(validate_arr[current_row_val-1]==1) { 
                worker_validation[param->id] = 0; 
                return NULL;
            } else {
                validate_arr[current_row_val-1] = 1;
            } 
        }
    }
    worker_validation[param->id] = 1;
}






int is_board_valid() {
    
    pthread_t* tid = (pthread_t*) malloc(sizeof(int)*NUM_OF_THREADS);
    pthread_attr_t attr;
    param_struct* params = (param_struct*)malloc(sizeof(param_struct)*NUM_OF_THREADS);
    worker_validation = (int*) malloc(sizeof(int)*NUM_OF_THREADS);
    int i =0;
    
    
    for(int row= 0;row<ROW_SIZE;row++) {
        
        params[i].id = i;
        params[i].starting_row = row;
        params[i].starting_col = 0;
        params[i].ending_row = row;
        params[i].ending_col = COL_SIZE;
        
        pthread_create(&(tid[i]), &attr, validate, &(params[i]));
        
    
        i++;

    }


    for(int col= 0;col<COL_SIZE;col++) {
        params[i].id = i;
        params[i].starting_row = 0;
        params[i].starting_col = col;
        params[i].ending_row = ROW_SIZE;
        params[i].ending_col = col;
        validate(&(params[i]));
        
        pthread_create(&(tid[i]), &attr, validate, &(params[i]));
        
        i++;

    }
    for(int row = 0; row<ROW_SIZE;row+=3) {
        for(int col =0;col<COL_SIZE;col+=3) {
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
        if(i<14) {
            break;
        }
        
        pthread_join(tid[i],NULL);
        
    }
    for(int i =0;i<NUM_OF_THREADS;i++) {
        // printf("i:%d val:%d \n",i,worker_validation[i]);

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







    


 








