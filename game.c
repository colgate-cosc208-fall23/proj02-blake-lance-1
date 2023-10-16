/*****
 * Project 02: Grid game
 * COSC 208, Introduction to Computer Systems, Fall 2023
 *****/

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>

int dim = 4;
//magic number 4?
//fgets instead of scan.
//exit option?
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
    printf("_____________________\n");
    for (int i = 0; i < dim; i++) {
        printf("______ ______ ______ ______\n");

        printf("|    | |    | |    | |    |\n");
        for (int j = 0; j < dim; j++) {
            if (grid[i][j].value < 10){
                printf("|  %d | ", grid[i][j].value);
            }
            else if (grid[i][j].value < 100){
                printf("| %d | ", grid[i][j].value);
            }
            else if (grid[i][j].value < 1000){
                printf("| %d| ", grid[i][j].value);
            }
            else if (grid[i][j].value < 10000){
                printf("|%d| ", grid[i][j].value);
            }
        }
        printf("\n");
        printf("|    | |    | |    | |    |\n");
        printf("‾‾‾‾‾‾ ‾‾‾‾‾‾ ‾‾‾‾‾‾ ‾‾‾‾‾‾\n");

    }
}

int has_valid_moves(struct Blocks **grid){
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (grid[i][j].filled == 0) {
                return 1;
            }
        }
    }
    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++) {
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
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (grid[i][j].value == 2048) {
                return 1;
            }
        }
    }
    return 0;
}

int upwards_column(int column[]){
    int up_column[dim];
    for (int i = 0; i<dim; i++){
        up_column[i] = 0;
    }
    int idp_idx = 0;
    int updated_score = 0;
    for (int i = 0; i<dim; i++){
        if (column[i] == 0) continue;  

        if (up_column[idp_idx] == 0){
            up_column[idp_idx] = column[i];
        }
        else if (up_column[idp_idx] == column[i]) {
            up_column[idp_idx] *= 2;
            updated_score = up_column[idp_idx];
            idp_idx++;
        } 
        else {
            idp_idx++;
            up_column[idp_idx] = column[i];
        }
    }
    for (int i = 0; i < dim; i++) {
        column[i] = up_column[i];
    }
    return updated_score;
}

int upwards_grid(struct Blocks **grid, int score){
    int return_score = score;
    for (int col = 0; col < dim; col++) {
        int column[dim];
        
        //int *column = malloc(sizeof(int) * 4);
        for (int row = 0; row < dim; row++) {
            column[row] = grid[row][col].value;
        }
        
        return_score += upwards_column(column);
        

        for (int row = 0; row < dim; row++) {
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
    return return_score;
}

int downwards_column(int column[]){
    int down_column[dim]; 
    for (int i = 0; i<dim; i++){
        down_column[i] = 0;
    }
    int idp_idx = 3; 
    int updated_score = 0;

    for (int i = 3; i >= 0; i--) {
        if (column[i] == 0) continue;  

        if (down_column[idp_idx] == 0) {
            down_column[idp_idx] = column[i];  
        } else if (down_column[idp_idx] == column[i]) {
            down_column[idp_idx] *= 2;  
            updated_score = down_column[idp_idx];
            idp_idx--; 
            down_column[idp_idx] = 0;  
        } else {
            idp_idx--;  
            down_column[idp_idx] = column[i];  
        }
    }

    for (int i = 0; i < dim; i++) {
        column[i] = down_column[i];
    }
    return updated_score;
}

int downwards_grid(struct Blocks **grid, int score){
    int return_score = score;

    for (int col = 0; col < dim; col++) {
        //int *column = malloc(sizeof(int) * 4);
        int column[dim];

        for (int row = 0; row < dim; row++) {
            column[row] = grid[row][col].value;
        }
        
        return_score += downwards_column(column);

        for (int row = 0; row < dim; row++) {
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
    return return_score;
}

int rightward_row(int row[], int score){
    int right_row[dim]; 
    for (int i = 0; i<dim; i++){
        right_row[i] = 0;
    }
    int idp_idx = 3; 
    int updated_score = 0;

    for (int i = 3; i >= 0; i--) {
        if (row[i] == 0) continue;  

        if (right_row[idp_idx] == 0) {
            right_row[idp_idx] = row[i];  
        } else if (right_row[idp_idx] == row[i]) {
            right_row[idp_idx] *= 2; 
            updated_score += right_row[idp_idx]; 
            idp_idx--; 
            right_row[idp_idx] = 0;  
        } else {
            idp_idx--;  
            right_row[idp_idx] = row[i];  
        }
    }

    for (int i = 0; i < dim; i++) {
        row[i] = right_row[i];
    }
    return updated_score;
}

int rightward_grid(struct Blocks **grid, int score){
    int return_score = score;
    for (int row = 0; row < dim; row++) {
        //int *rows = malloc(sizeof(int) * 4);
        int rows[dim];

        for (int col = 0; col < dim; col++) {
            rows[col] = grid[row][col].value;
        }
        
        score += rightward_row(rows, score);

        for (int col = 0; col < dim; col++) {
            grid[row][col].value = rows[col];
            if (grid[row][col].value == 0){
                grid[row][col].filled = 0;
            }
            else{
                grid[row][col].filled = 1;
            }
        }
    }
    return return_score;
}

int leftward_row(int row[]){
    int left_row[dim];
    for (int i = 0; i<dim; i++){
        left_row[i] = 0;
    }
    int idp_idx = 0;
    int updated_score = 0;
    for (int i = 0; i<dim; i++){
        if (row[i] == 0) continue;  

        if (left_row[idp_idx] == 0){
            left_row[idp_idx] = row[i];
        }
        else if (left_row[idp_idx] == row[i]) {
            left_row[idp_idx] *= 2;
            updated_score+=left_row[idp_idx];
            idp_idx++;
        } 
        else {
            idp_idx++;
            left_row[idp_idx] = row[i];
        }
    }
    for (int i = 0; i < dim; i++) {
        row[i] = left_row[i];
    }
    return updated_score;
}

int leftwards_grid(struct Blocks **grid, int score){
    int return_score = score;
    for (int row = 0; row < dim; row++) {
        
        //int *rows = malloc(sizeof(int) * 4);
        int rows[dim];
        for (int col = 0; col < dim; col++) {
            rows[col] = grid[row][col].value;
        }
        
        return_score += leftward_row(rows);

        for (int col = 0; col < dim; col++) {
            grid[row][col].value = rows[col];
            if (grid[row][col].value == 0){
                grid[row][col].filled = 0;
            }
            else{
                grid[row][col].filled = 1;
            }
        }
    }
    return return_score;
}

void random_generation(struct Blocks **grid){
    struct Dimensions collection[16];
    int counter = 0;
    for (int row = 0; row < dim; row++){
        for (int col = 0; col < dim; col++){
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

int is_effective_move(struct Blocks **grid, char move){
    int flag = 0;
    struct Blocks **alias = init_grid(dim,dim);
    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            alias[i][j].value = grid[i][j].value;
        }
    }
    if (move == 'w'){
        upwards_grid(alias, 0);
    }
    else if (move == 'a'){
        leftwards_grid(alias,0);
    }
    else if (move == 's'){
        downwards_grid(alias,0);
    }
    else if (move == 'd'){
        rightward_grid(alias,0);
    }
    for (int i = 0; i<dim; i++){
        for (int j = 0; j < dim; j++){
            if (alias[i][j].value != grid[i][j].value){
                flag = 1;
            }
        }
    }
    free_grid(alias, dim);
    return flag;
}

int main() {
    // Quote from the 1983 film WarGames
    // Watch the movie for free on Pluto TV
    // https://pluto.tv/en/on-demand/movies/5e3c8d1a86e96850bcc4a88f

    struct Blocks **grid = init_grid(dim,dim);
    int flag = has_valid_moves(grid);
    int win = obtained_2048(grid);
    int rounds = 1;
    int score = 0;
    //printf("%d\n", flag);
    //display_grid(grid);
    random_generation(grid);
    random_generation(grid);
    

    while (flag == 1 && win==0){
        printf("Round #%d     Score: %d\n", rounds, score);
        display_grid(grid);

        printf("Enter a letter: (w(up)/a(left)/s(down)/d(right))\n");

        char choice[10];//10 character should be enough to store input
        fgets(choice, 10, stdin);
        choice[strlen(choice)-1] = '\0';
        printf("Your move: %s\n", choice);
        //printf("%ld",strlen(choice));
        if (!strcmp(choice,"w") || !strcmp(choice,"a") || !strcmp(choice,"d") || !strcmp(choice,"s")){
            int moves = is_effective_move(grid, choice[0]);
            //printf("Moveable: %d\n", moves);
            if (moves){
                if (!strcmp(choice,"w")){
                    score = upwards_grid(grid, score);
                }
                else if (!strcmp(choice,"a")){
                    score = leftwards_grid(grid, score);
                }
                else if (!strcmp(choice,"d")){
                    score = rightward_grid(grid, score);
                }
                else if (!strcmp(choice,"s")){
                    score = downwards_grid(grid, score);
                }
                random_generation(grid);
                flag = has_valid_moves(grid);
                win = obtained_2048(grid);
            }
            else{
                printf("=====================\n");
                printf(" Tiles did not change\n");
                printf("=====================\n");
            }
        }
        else{
            printf("Invalid input, try again.\n");
        }
        rounds++;
    }

    if (win == 1){
        printf("You win\n");
    }
    else{
        printf("Oh no. You are dead.\nbetter luck next time\n");
    }

    free_grid(grid, dim);

    
    printf("A STRANGE GAME.\n");
    printf("THE ONLY WINNING MOVE IS NOT TO PLAY.\n");

    

}


