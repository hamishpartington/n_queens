#include "extension.h"

int main(int argc, char* argv[])
{
  test();

  if(argc != ARGS){
    printf("Usage: %s <number of queens (N)>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if(strlen(argv[argc-1]) > MAX_N_STRING){
    printf("N must be an integer between %i and %i\n", MIN_N, MAX_N);
    exit(EXIT_FAILURE);
  }

  int n = argv_to_n(argv[argc-1]);

  if(!( n >= MIN_N && n <= MAX_N)){
    printf("N must be an integer between %i and %i\n", MIN_N, MAX_N);
    exit(EXIT_FAILURE);
  }

  int k = 0;
  // size of 1 d array of permutations 1 to n
  long size = n_factorial(n)*n;
  int* perm_base = init(n, size);
  int* perm_list = (int*)neill_calloc(size, sizeof(int));
  
  k = perm(perm_base, 0,  n, perm_list, k);

  free(perm_base);

  root* r = (root*)neill_calloc(1, sizeof(root));

  r = solve(perm_list, n, k, r);

  free(perm_list);

  int n_sols = r->n_sols;

  printf("\n");
  printf("%i solutions\n\nPress any key to display the next solution\n\n", n_sols);

  board* b = r->start;
  if(n_sols > 0){
    SDL_display_sols(n, b, n_sols);
  }

  free_list(r);

  return 0;
  
}

int argv_to_n(char* argv)
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

int* init(int n, long size)
{
  int* perm_base = (int*)neill_calloc(size, sizeof(int));
  int j = 1, i = 0;

  for(; j <=n; i++, j++){
    perm_base[i] = j;
  }

  return perm_base;
} 

long n_factorial(int n)
{ 
  int fact = 1;
  for(int i = 1; i <= n; i++){
    fact *= i;
  }
  return fact;
}

void swap(int* a, int* b)
{
  int temp;

  temp = *a;
  *a = *b;
  *b = temp;
}

int perm(int* perm_base, int start, int last, int* perm_list, int k)
{
  int i;

  if(start == last){
    for(i = 0; i < last; i++){
      perm_list[k++] = perm_base[i];
    }
  }else{
    for(i = start; i < last; i++){
      swap(perm_base + start, perm_base + i);
      k = perm(perm_base, start + 1, last, perm_list, k);
      swap(perm_base + start, perm_base + i);
    }
  }
  return k;
}

root* solve(int* perm_list, int n, int k, root* r)
{ 

  int* permu, n_sols = 0;
  board* current = NULL;

  permu = (int*)neill_calloc(n, sizeof(int));
   
  for(int i = 0; i < k/n; i++){
    for(int j = 0; j < n; j++){
       permu[j] = perm_list[(i*n)+j];
    }
    if(is_sol(permu, n)){
     n_sols++;
      if(n_sols == 1){
        r->start = allocate_data(permu, n);
        current = r->start;
      }else{
        current->next = allocate_data(permu, n);
        current = current->next;
      }
    }
  }
  r->n_sols = n_sols;
  free(permu);
  return r;
}

board* allocate_data(int* sol, int n)
{
  board* p;

  p = (board*)neill_calloc(1, sizeof(board));

  for(int i = 0; i < n; i++){
    p->q_array[i] = sol[i];
  }
  p->next = NULL;

  return p;
}

bool is_sol(int* permu, int n)
{
  int dy, dx;

  for(int i = 0; i <n; i++){
    for(int j = 0; j <n; j++){
      dy = permu[i] - permu[j];
      dx = i - j;
      if(!(dx == 0 && dy == 0) && (dx == dy || dx == -dy)){
        return false;
      }
    }
  }
  return true;
}

void print_sol(int* sol, int n)
{
  for(int i = 0; i < n; i++){
    printf("%X", sol[i]);
  }
  printf("\n");
}

bool free_list(root* r)
{
  if(r){
    board* tmp;
    board* p = r->start;
    while(p!=NULL){
      tmp = p->next;
      free(p);
      p = tmp;
    }
   free(r);
  return true;
  }
  return false;
}


void* neill_calloc(int n, size_t size)
{
   void* v = calloc(n, size);
   if(v==NULL){
    fprintf(stderr, "Cannot calloc() space\n");
    exit(EXIT_FAILURE);
   }
   return v;
}

int SDL_display_sols(int n, board* b, int n_sols)
{
  // initialise SDL modules
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  // creates window
  SDL_Window* window = SDL_CreateWindow("Solutions", 
                                        // x position
                                        SDL_WINDOWPOS_CENTERED, 
                                        // y position
                                        SDL_WINDOWPOS_CENTERED, 
                                        //width
                                        (n*CHECKER_SIZE),
                                        //height 
                                        (n*CHECKER_SIZE), 
                                        SDL_WINDOW_OPENGL);
  //creates texture
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  
  bool quit = false;
  int count = 1;
  
  SDL_make_board(n, b, renderer, count);
  while(!quit){
    SDL_Event e;
      if (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT || (count == n_sols && n != 1) || 
           (count > n_sols && n == 1)){
          quit = true;
        }
        if(e.type == SDL_KEYUP){
          b = b->next;
          count++;
          if(n != 1){
            SDL_make_board(n, b, renderer, count);
          }
        }
      }
    SDL_RenderPresent(renderer);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

void SDL_make_board(int n, board* b, SDL_Renderer* renderer, int count)
{ 
  SDL_Rect square = {0, 0, CHECKER_SIZE, CHECKER_SIZE};
  SDL_Rect queen  = {QUEEN_OFFSET, QUEEN_OFFSET, QUEEN_SIZE, QUEEN_SIZE};
  bool white = true;
  int qy;
  int* sol = b->q_array;
  
  printf("Solution %i: ", count);
  print_sol(sol, n);

  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(!white){
        SDL_SetRenderDrawColor(renderer, BLACK, OPAQUE);
      }else{
        SDL_SetRenderDrawColor(renderer, WHITE, OPAQUE);
      }
      SDL_RenderFillRect(renderer, &square);

      if((sol[i] - 1 == j)){
      //red queen
        SDL_SetRenderDrawColor(renderer, RED, OPAQUE);
        qy = j*CHECKER_SIZE + QUEEN_OFFSET;
        queen.y = qy;
        SDL_RenderFillRect(renderer, &queen);
      }
      square.y += CHECKER_SIZE;
      /*alternate black white squares except for when starting new column on 
        boards where n is even*/
      if(!(j == (n-ADJUST) && n % EVEN == 0)){
        white = !white;
      }
    }
    square.y = 0;
    square.x += CHECKER_SIZE;
    queen.x += CHECKER_SIZE;
  }
}


void test(void)
{
  assert(argv_to_n("10") == 10);
  assert(argv_to_n("3") == 3);

  int n = 8; 
  assert(n_factorial(n) == 40320);
  
  long size = n_factorial(n)*n;
  int* perm_base = init(n, size);
  int* perm_list = (int*)neill_calloc(size, sizeof(int));

  int permu[MAX_N] = {10, 1, 6, 4, 2, 8, 3, 9, 7, 5};
  assert(is_sol(permu, 10));
  
  int k = 0;

  k = perm(perm_base, 0,  n, perm_list, k);
  //perm returns n!*n if working correctly 
  assert(k == 322560);

  free(perm_base);

  root* r = (root*)neill_calloc(1, sizeof(root));

  board* b = allocate_data(permu, 10);

  assert(b->q_array[0] == 10);
  assert(b->q_array[1] == 1);
  assert(b->q_array[5] == 8);
  assert(b->q_array[9] == 5);

  free(b);

  r = solve(perm_list, n, k, r);

  assert(r->n_sols == 92);

  free(perm_list);

  free_list(r);

}

