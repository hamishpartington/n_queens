#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define MAX_N 10
#define MIN_N 1
#define MAX_N_STRING 20
#define MIN_ARGS 2
#define MAX_ARGS 3
#define ZERO_ASCII 48
#define MAX_LIST 1000000
#define ADJUST 1
#define MAX_QSTRING 11

//data structure
struct board{
  int index;
  int n_queens;
  char q_string[MAX_QSTRING];
  bool is_solution;
};

typedef struct board board;

void test(void);
// converts argv n input to an integer
int argv_to_n(char argv[MAX_N_STRING]);
//initiates board_list
void board_init(board board_list[MAX_LIST], int n);
//return true if child is already in list
bool in_list(board child, board board_list[MAX_LIST], int f);
//return true if queen to be added is in the same column as another queen
bool same_col(board parent, int col);
//return true if queen to be added is on the same row as another queen
bool same_row(board parent, int row, int n);
//return true if queen to be added is on the same diagonal as another queen
bool same_diag(board parent, int row, int col, int n);
//creates child board with queen added at row and col
board add_queen(board parent, int row, int col, int n, int end);
//generates board_list of unique children and prints solutions
int add_children(board board_list[MAX_LIST], int n, char sols[MAX_LIST][MAX_N]);
//checks whether child board already exists in board_list
bool is_unique(board board_list[MAX_LIST], board child, int f, int end);


