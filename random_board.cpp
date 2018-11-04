/*
Name: Jackson Goble
Class: CSCI4350
Assignment: OLA1
Due date: 10/2/2018

Description: This program is used in conjuntion with the output from
    random_board to solve the 8-puzzle problem using A* with 4
    different heuristic functions.

Usage: make
     : ./random_board seed moves
*/
#include <iostream>
#include <fstream>

using namespace std;

static const int board_width = 4;
static const int board_height = 4;

int main(int argc, char* argv[]) {
    int board[board_width][board_height];
    int i = 0, j = 0, k = 0;
    int random_number = 0;
    int zero_x = 0, zero_y = 0;

    if(argc != 3) {
        cout << "USAGE: ./random_board seed moves" << endl;
        exit(1);
    }
    
    srand(atoi(argv[1]));
    //set the initial board state to whatever state is given in stdin
    for(j = 0; j < board_height; j++) {
        for(i = 0; i < board_width; i++) {
            cin >> board[i][j];
            //if the 0 is found, track its coordinates
            if(board[i][j] == 0) {
                zero_x = i;
                zero_y = j;
            }
        }
    }


    for(i = 0; i < atoi(argv[2]); i++) { //make argv[2] number of moves

        while(1) { // generate random numbers until a valid move is made
            random_number = rand()%4;

            //move space up
            if(random_number == 0 && zero_y > 0) {
                board[zero_x][zero_y] = board[zero_x][zero_y-1];
                board[zero_x][zero_y-1] = 0;
                zero_y--;
                break;
            }
            //move space right
            if(random_number == 1 && zero_x < board_width - 1) {
                board[zero_x][zero_y] = board[zero_x+1][zero_y];
                board[zero_x+1][zero_y] = 0;
                zero_x++;
                break;
            }
            //move space down
            if(random_number == 2 && zero_y < board_height - 1) {
                board[zero_x][zero_y] = board[zero_x][zero_y+1];
                board[zero_x][zero_y+1] = 0;
                zero_y++;
                break;
            }
            //move space left
            if(random_number == 3 && zero_x >  0) {
                board[zero_x][zero_y] = board[zero_x-1][zero_y];
                board[zero_x-1][zero_y] = 0;
                zero_x--;
                break;
            }
        }
    }

    //print the board to std out
    for(j = 0; j < board_height; j++) {
        for(i = 0; i < board_width; i++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
