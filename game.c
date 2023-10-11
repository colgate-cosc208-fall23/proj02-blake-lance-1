/*****
 * Project 02: Grid game
 * COSC 208, Introduction to Computer Systems, Fall 2023
 *****/

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

struct Blocks{
    int value;
    int filled;
};

struct Blocks** init_grid(int rows, int cols){
    struct Blocks **grid = malloc(sizeof(struct Blocks *) * rows);
    for (int i = 0; i < 4; i++){
        grid[i] = malloc(sizeof(struct Blocks *)*cols);
        for (int j = 0; j < 4; j++){
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
        int *column = malloc(sizeof(int) * 4);
        for (int row = 0; row < 4; row++) {
            column[row] = grid[row][col].value;
        }
        
        upwards_column(column);
        

        for (int row = 0; row < 4; row++) {
            grid[row][col].value = column[row];
        }
        free(column);
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
        int *column = malloc(sizeof(int) * 4);
        for (int row = 0; row < 4; row++) {
            column[row] = grid[row][col].value;
        }
        
        downwards_column(column);

        for (int row = 0; row < 4; row++) {
            grid[row][col].value = column[row];
        }
        free(column);
    }
}

int main() {
    // Quote from the 1983 film WarGames
    // Watch the movie for free on Pluto TV
    // https://pluto.tv/en/on-demand/movies/5e3c8d1a86e96850bcc4a88f

    struct Blocks **grid = init_grid(4,4);
    int flag = has_valid_moves(grid);
    //int win = obtained_2048(grid);
    printf("%d\n", flag);
    grid[0][0].value = 4;
    grid[0][0].filled = 1;
    grid[1][0].value = 2;
    grid[1][0].filled = 1;
    grid[2][0].value = 16;
    grid[2][0].filled = 1;
    grid[3][0].value = 8;
    grid[3][0].filled = 1;

    grid[0][1].value = 2;
    grid[0][1].filled = 1;
    grid[1][1].value = 4;
    grid[1][1].filled = 1;
    grid[2][1].value = 32;
    grid[2][1].filled = 1;
    grid[3][1].value = 256;
    grid[3][1].filled = 1;


    grid[0][2].value = 16;
    grid[1][2].value = 32;
    grid[2][2].value = 64;
    grid[3][2].value = 128;
    grid[0][2].filled = 1;
    grid[1][2].filled = 1;
    grid[2][2].filled = 1;
    grid[3][2].filled = 1;


    grid[0][3].value = 256;
    grid[1][3].value = 512;
    grid[2][3].value = 32;
    grid[3][3].value = 1024;
    grid[0][3].filled = 1;
    grid[1][3].filled = 1;
    grid[2][3].filled = 1;
    grid[3][3].filled = 1;


    flag = has_valid_moves(grid);
    display_grid(grid);
    printf("%d\n", flag);



    printf("A STRANGE GAME.\n");
    printf("THE ONLY WINNING MOVE IS NOT TO PLAY.\n");

}