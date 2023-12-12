#include "8q.h"

int main(int argc, char* argv[])
{
  test();

  if(!(argc == MIN_ARGS || argc == MAX_ARGS)){
    printf("Usage: %s <n>\nor,\nUsage: %s -verbose <n>\n", argv[0], argv[0]);
    exit(EXIT_FAILURE);
  }

  if(strlen(argv[argc-1]) > 2){
    printf("N must be an integer between %i and %i\n", MIN_N, MAX_N);
    exit(EXIT_FAILURE);
  }

  int n = argv_to_n(argv[argc-1]);

  if(!( n >= MIN_N && n <= MAX_N)){
    printf("N must be an integer between %i and %i\n", MIN_N, MAX_N);
    exit(EXIT_FAILURE);
  }

  // array of boards
  static board board_list[MAX_LIST];
  static char sols[MAX_LIST][MAX_N];
  board_init(board_list, n);

  int n_sols = add_children(board_list, n, sols);

  if(n_sols > 0 && strcmp(argv[1], "-verbose") == 0){
    for(int i = 0; i < n_sols; i ++){
      printf("%s\n", sols[i]);
    }
  }

  printf("%i solutions\n", n_sols);

  return 0;
  
}

int argv_to_n(char argv[MAX_N_STRING])
{
  int str_length, tn = 0, un, n;

  str_length = strlen(argv);

  if(str_length == 2){
    tn = (int)argv[0] - ZERO_ASCII;
    un = (int)argv[1] - ZERO_ASCII;
  }else{
    un = (int)argv[0] - ZERO_ASCII;
  }
 
  n = tn*10 + un;

  return n;
}

void board_init(board board_list[MAX_LIST], int n)
{
  board_list[0].index = 0;
  board_list[0].n_queens = 0;
  board_list[0].is_solution = false;

  for(int i = 0; i < MAX_N; i++){
    board_list[0].q_string[i] = '\0'; 
  }

  for(int i = 0; i < n; i++){
    board_list[0].q_string[i] = '0';
  }
}

bool in_list(board child, board board_list[MAX_LIST], int f)
{
  for(int i = 0; i <= f; i++){
    if(strcmp(child.q_string, board_list[i].q_string) == 0){
      return true;
    }
  }
  return false;
}

bool same_col(board parent, int col)
{
  if(parent.q_string[col] != '0'){
      return true;
  }
  return false;
}

bool same_row(board parent, int row, int n)
{
  int string_row;

  for(int i = 0; i < n; i++){
    string_row = (int)parent.q_string[i] - ZERO_ASCII;
    // to catch 10s
    if(parent.q_string[i] == 'A'){
      string_row = 10;
    }
    if((string_row - ADJUST) == row){
      return true;
    }
  }
  return false;
}

bool same_diag(board parent, int row, int col, int n)
{
  int string_row, string_col;
  int dy, dx;

  for(int i = 0; i < n; i++){
    if(parent.q_string[i] != '0'){
      string_row = (int)parent.q_string[i] - ZERO_ASCII;
      // to catch 10s
      if(parent.q_string[i] == 'A'){
      string_row = 10;
      }
      string_row = string_row - ADJUST;
      string_col = i;
      dy = row - string_row;
      dx = col - string_col;
      if(dx == dy || dx == -dy){
        return true;
      }
    }
  }
  return false;
}

board add_queen(board parent, int row, int col, int n, int end)
{
  board child = parent;
  
  row = row + ADJUST;
  if(row == 10){
    child.q_string[col] = 'A';
  }else{
    child.q_string[col] = (char)(row + ZERO_ASCII);
  }
  child.n_queens++;
  child.index = end;

  if(child.n_queens == n){
    child.is_solution = true;
  }
  return child; 
}

bool is_unique(board board_list[MAX_LIST], board child, int f, int end)
{
  for(int i = f; i < end; i++){
    if(strcmp(board_list[i].q_string, child.q_string) == 0){
      return false;
    }
  }
  return true;
}

int add_children(board board_list[MAX_LIST], int n, char sols[MAX_LIST][MAX_N])
{
  int f = 0, n_queens = 1, n_sols = 0, end = 1;
  board child;

  while(n_queens <= n){
    for(f = f; f < end; f++){
      for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
          child = add_queen(board_list[f], i, j, n, end);
          if((!same_col(board_list[f], j) && !same_row(board_list[f], i, n) 
            && !same_diag(board_list[f], i, j, n))
            && is_unique(board_list, child, f, end)){
            board_list[end] = add_queen(board_list[f], i, j, n, end);
            if(board_list[end].is_solution){
              strcpy(sols[n_sols], board_list[end].q_string);
              n_sols++;
            }
            end++;
          }
        }
      }
    }
    n_queens++;
  }
  return n_sols;
}

void test(void)
{
  assert(argv_to_n("10") == 10);
  assert(argv_to_n("3") == 3);

  static board board_list[MAX_LIST];
  board_init(board_list, 6);
  assert(board_list[0].index == 0);
  assert(board_list[0].n_queens == 0);
  assert(strcmp(board_list[0].q_string, "000000") == 0);
  assert(in_list(board_list[0], board_list, 0));
  assert(!same_col(board_list[0], 3));
  assert(!same_row(board_list[0], 3, 6));

  board test_board;
  test_board.index = 1;
  test_board.n_queens = 5;
  strcpy(test_board.q_string, "362510");
  assert(same_col(test_board, 3));
  assert(!same_col(test_board, 5));
  assert(same_row(test_board, 5, 6));
  assert(!same_row(test_board, 3, 6));
  assert(same_diag(test_board, 4, 2, 6));
  assert(!same_diag(test_board, 3, 5, 6));

  board new_board;
  new_board = add_queen(test_board, 3, 5, 6, 1);
  assert(strcmp(new_board.q_string, "362514") == 0);

  assert(new_board.n_queens == 6);
  assert(new_board.is_solution);

  assert(is_unique(board_list, new_board, 0, 1));
  board_list[1] = add_queen(test_board, 3, 5, 6, 1);
  assert(!is_unique(board_list, new_board, 0, 2));

  static char sols[MAX_LIST][MAX_N];

  assert(add_children(board_list, 1, sols) == 1);
  assert(add_children(board_list, 4, sols) == 2);
  assert(add_children(board_list, 3, sols) == 0);

  // test A for 10 is working
  board_init(board_list, 10);

  board test_board2;
  test_board2.index = 1;
  test_board2.n_queens = 9;
  strcpy(test_board2.q_string, "3625147890");
  
  assert(!same_row(test_board2, 9, 10));
  assert(!same_col(test_board2, 9));
  new_board = add_queen(test_board2, 9, 9, 10, 1);
  assert(strcmp(new_board.q_string, "362514789A") == 0);
}



