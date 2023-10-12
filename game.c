/*****
 * Project 02: Grid game
 * COSC 208, Introduction to Computer Systems, Fall 2023
 *****/

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>

struct Blocks{
    int value;
    int filled;
};

struct Dimensions{
    int rows;
    int cols;
};

struct Blocks** init_grid(int rows, int cols){
    struct Blocks **grid = malloc(sizeof(struct Blocks *) * rows);
    for (int i = 0; i < rows; i++){
        grid[i] = malloc(sizeof(struct Blocks *)*cols);
        for (int j = 0; j < cols; j++){
            grid[i][j].value = 0;
            grid[i][j].filled = 0;
        }
    }
    return grid;
}

void display_grid(struct Blocks **grid) {
    printf("_________________\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("| %d ", grid[i][j].value);
        }
        printf("|\n");
    }
    printf("_________________\n");
}

int has_valid_moves(struct Blocks **grid){
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (grid[i][j].filled == 0) {
                return 1;
            }
        }
    }
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++) {
            int val = grid[i][j].value;

            // Check above
            if (i > 0 && grid[i - 1][j].value == val) {
                return 1;
            }
            // Check below
            if (i < 3 && grid[i + 1][j].value == val) {
                return 1;
            }
            // Check left
            if (j > 0 && grid[i][j - 1].value == val) {
                return 1;
            }
            // Check right
            if (j < 3 && grid[i][j + 1].value == val) {
                return 1;
            }
        }
    }
    return 0;
}

int obtained_2048(struct Blocks **grid){
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (grid[i][j].value == 2048) {
                return 1;
            }
        }
    }
    return 0;
}

void upwards_column(int column[]){
    int up_column[4] = {0,0,0,0};
    int idp_idx = 0;
    for (int i = 0; i<4; i++){
        if (column[i] == 0) continue;  

        if (up_column[idp_idx] == 0){
            up_column[idp_idx] = column[i];
        }
        else if (up_column[idp_idx] == column[i]) {
            up_column[idp_idx] *= 2;
            idp_idx++;
        } 
        else {
            idp_idx++;
            up_column[idp_idx] = column[i];
        }
    }
    for (int i = 0; i < 4; i++) {
        column[i] = up_column[i];
    }
}

void upwards_grid(struct Blocks **grid){
    for (int col = 0; col < 4; col++) {
        int column[4];
        //int *column = malloc(sizeof(int) * 4);
        for (int row = 0; row < 4; row++) {
            column[row] = grid[row][col].value;
        }
        
        upwards_column(column);
        

        for (int row = 0; row < 4; row++) {
            grid[row][col].value = column[row];
            if (grid[row][col].value == 0){
                grid[row][col].filled = 0;
            }
            else{
                grid[row][col].filled = 1;
            }
        }
        //free(column);
    }
}

void downwards_column(int column[]){
    int down_column[4] = {0, 0, 0, 0}; 
    int idp_idx = 3; 

    for (int i = 3; i >= 0; i--) {
        if (column[i] == 0) continue;  

        if (down_column[idp_idx] == 0) {
            down_column[idp_idx] = column[i];  
        } else if (down_column[idp_idx] == column[i]) {
            down_column[idp_idx] *= 2;  
            idp_idx--; 
            down_column[idp_idx] = 0;  
        } else {
            idp_idx--;  
            down_column[idp_idx] = column[i];  
        }
    }

    for (int i = 0; i < 4; i++) {
        column[i] = down_column[i];
    }
}

void downwards_grid(struct Blocks **grid){
    for (int col = 0; col < 4; col++) {
        //int *column = malloc(sizeof(int) * 4);
        int column[4];

        for (int row = 0; row < 4; row++) {
            column[row] = grid[row][col].value;
        }
        
        downwards_column(column);

        for (int row = 0; row < 4; row++) {
            grid[row][col].value = column[row];
            if (grid[row][col].value == 0){
                grid[row][col].filled = 0;
            }
            else{
                grid[row][col].filled = 1;
            }
        }
        //free(column);
    }
}

void rightward_row(int row[]){
    int right_row[4] = {0, 0, 0, 0}; 
    int idp_idx = 3; 

    for (int i = 3; i >= 0; i--) {
        if (row[i] == 0) continue;  

        if (right_row[idp_idx] == 0) {
            right_row[idp_idx] = row[i];  
        } else if (right_row[idp_idx] == row[i]) {
            right_row[idp_idx] *= 2;  
            idp_idx--; 
            right_row[idp_idx] = 0;  
        } else {
            idp_idx--;  
            right_row[idp_idx] = row[i];  
        }
    }

    for (int i = 0; i < 4; i++) {
        row[i] = right_row[i];
    }
}

void rightward_grid(struct Blocks **grid){
    for (int row = 0; row < 4; row++) {
        //int *rows = malloc(sizeof(int) * 4);
        int rows[4];

        for (int col = 0; col < 4; col++) {
            rows[col] = grid[row][col].value;
        }
        
        rightward_row(rows);

        for (int col = 0; col < 4; col++) {
            grid[row][col].value = rows[col];
            if (grid[row][col].value == 0){
                grid[row][col].filled = 0;
            }
            else{
                grid[row][col].filled = 1;
            }
        }
        //free(rows);
    }
}

void leftward_row(int row[]){
    int left_row[4] = {0,0,0,0};
    int idp_idx = 0;
    for (int i = 0; i<4; i++){
        if (row[i] == 0) continue;  

        if (left_row[idp_idx] == 0){
            left_row[idp_idx] = row[i];
        }
        else if (left_row[idp_idx] == row[i]) {
            left_row[idp_idx] *= 2;
            idp_idx++;
        } 
        else {
            idp_idx++;
            left_row[idp_idx] = row[i];
        }
    }
    for (int i = 0; i < 4; i++) {
        row[i] = left_row[i];
    }
}

void leftwards_grid(struct Blocks **grid){
    for (int row = 0; row < 4; row++) {
        
        //int *rows = malloc(sizeof(int) * 4);
        int rows[4];
        for (int col = 0; col < 4; col++) {
            rows[col] = grid[row][col].value;
        }
        
        leftward_row(rows);

        for (int col = 0; col < 4; col++) {
            grid[row][col].value = rows[col];
            if (grid[row][col].value == 0){
                grid[row][col].filled = 0;
            }
            else{
                grid[row][col].filled = 1;
            }
        }
        //free(rows);
    }
}

void random_generation(struct Blocks **grid){
    struct Dimensions collection[16];
    int counter = 0;
    for (int row = 0; row < 4; row++){
        for (int col = 0; col < 4; col++){
            if (grid[row][col].filled == 0){
                collection[counter].rows = row;
                collection[counter].cols = col;
                counter++;
            }
        }
    }
    int random_num = rand() % counter - 1;
    if (random_num < 0){
        random_num = 0;
    }
    float two_four_decider = (float)rand() / (float)RAND_MAX;
    if (two_four_decider >= 0.9){
        grid[collection[random_num].rows][collection[random_num].cols].value = 4;
    }
    else{
        grid[collection[random_num].rows][collection[random_num].cols].value = 2;
    }
    grid[collection[random_num].rows][collection[random_num].cols].filled = 1;

}

void free_grid(struct Blocks **grid, int rows) {
    for (int i = 0; i < rows; i++) {
        free(grid[i]);
    }
    free(grid);
}

int main() {
    // Quote from the 1983 film WarGames
    // Watch the movie for free on Pluto TV
    // https://pluto.tv/en/on-demand/movies/5e3c8d1a86e96850bcc4a88f

    struct Blocks **grid = init_grid(4,4);
    int flag = has_valid_moves(grid);
    int win = obtained_2048(grid);
    int rounds = 1;
    printf("%d\n", flag);
    display_grid(grid);
    random_generation(grid);
    random_generation(grid);
    

    while (flag == 1 || win==0){
        printf("Round #%d\n", rounds);
        display_grid(grid);

        printf("Enter a letter: (w/a/s/d)\n");

        char choice;
        scanf(" %c", &choice);
        printf("Your move: %c\n", choice);
        if (choice == 'w' || choice == 'a' || choice == 'd' || choice == 's'){
            if (choice == 'w'){
                upwards_grid(grid);
            }
            else if (choice == 'a'){
                leftwards_grid(grid);
            }
            else if (choice == 'd'){
                rightward_grid(grid);
            }
            else if (choice == 's'){
                downwards_grid(grid);
            }
            random_generation(grid);
            flag = has_valid_moves(grid);
            win = obtained_2048(grid);
        }
        else{
            printf("Invalid input, try again.\n");
        }
        rounds++;
    }

    if (flag == 0 && win == 1){
        printf("You win\n");
    }
    else{
        printf("better luck next time\n");
    }

    free_grid(grid, 4);

    /*
    grid[0][0].value = 2;
    grid[1][0].value = 2;
    grid[2][0].value = 4;
    grid[3][0].value = 8;

    grid[0][1].value = 2;
    grid[1][1].value = 0;
    grid[2][1].value = 2;
    grid[3][1].value = 4;


    grid[0][2].value = 4;
    grid[1][2].value = 4;
    grid[2][2].value = 4;
    grid[3][2].value = 0;


    grid[0][3].value = 8;
    grid[1][3].value = 4;
    grid[2][3].value = 2;
    grid[3][3].value = 2;


    flag = has_valid_moves(grid);
    display_grid(grid);
    printf("%d\n", flag);

    rightward_grid(grid);
    display_grid(grid);

    rightward_grid(grid);
    display_grid(grid);

    rightward_grid(grid);
    display_grid(grid);

    downwards_grid(grid);
    display_grid(grid);

    leftwards_grid(grid);
    display_grid(grid);

    downwards_grid(grid);
    display_grid(grid);

    rightward_grid(grid);
    display_grid(grid);

    downwards_grid(grid);
    display_grid(grid);

    leftwards_grid(grid);
    display_grid(grid);

    downwards_grid(grid);
    display_grid(grid);

    printf("_________________\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("| %d ", grid[i][j].filled);
        }
        printf("|\n");
    }
    printf("_________________\n");

    */

    printf("A STRANGE GAME.\n");
    printf("THE ONLY WINNING MOVE IS NOT TO PLAY.\n");

    

}


