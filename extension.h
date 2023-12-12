#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <SDL.h>
#include <math.h>

#define MAX_N 10
#define MIN_N 1
#define MAX_N_STRING 2
#define ARGS 2
#define ZERO_ASCII 48
#define CHECKER_SIZE 80
#define QUEEN_SIZE 60
#define QUEEN_OFFSET 10
#define ADJUST 1
#define EVEN 2
#define BLACK 0,0,0
#define WHITE 255,255,255
#define RED 255,0,0
#define OPAQUE 255

//data structure
struct board{
  int q_array[MAX_N];
  struct board* next;
};

typedef struct board board;

struct root {
  board* start;
  int n_sols;
};

typedef struct root root;

void test(void);
//converts argv n input to an integer
int argv_to_n(char* argv);
//initites string of numbers 1 to n to feed into perm
int* init(int n, long size);
//stores all permutations of 1 to end in a 1D array
int perm(int* perm_base, int start, int last, int* perm_list, int k);
//swap function
void swap(int *a, int *b);
//puts solutions into linked list;
board* allocate_data(int* sol, int n);
//solve the problem
root* solve(int* perm_list, int n, int k, root* r);
/*return true if permutation is a solution: just checking diagonals of each 
queen against other queens as we already know they wont be on the same row
or column as the string only contains one instance of each number*/
bool is_sol(int* permu, int n);
//prints solutions
void print_sol(int* sol, int n);
//frees memory used for list
bool free_list(root* r);
//Stolen from Neill (https://github.com/csnwc/ADTs/blob/main/General/general.c)
void* neill_calloc(int n, size_t size);
// calc factorial of n
long n_factorial(int n);
//function which creates appropriately sized SDL window in which to render solutions
int SDL_display_sols(int n, board* b, int n_sols);
/*function which renders a solution on a chess board with red squares representing
queens*/
void SDL_make_board(int n, board* b, SDL_Renderer* renderer, int count);

