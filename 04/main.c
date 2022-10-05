#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>

#define MAX_NUM_OF_BOARD 100
#define MAX_GAME_LEN 1000
#define SKIP_LINE(data) while(*data == '\n') data++;

typedef struct{
  char* data;
  int size;
} File;


File Load_file(const char* file_path){
  struct stat stats;
  if(stat(file_path, &stats)){
    fprintf(stderr,"ERROR: Can't get the stats of file\n");
    exit(1);
  }

  int size = stats.st_size;
  char* buf = malloc(sizeof(char) * size);
  if(buf == NULL){
    fprintf(stderr,"ERROR: malloc failure\n");
    exit(1);
  }
    
  FILE* file = fopen(file_path,"r");
  if(file == NULL){
    fprintf(stderr,"ERROR: Could not open %s file\n",file_path);
  }
  if(fread(buf, 1, size, file) != (size_t) size){
    fprintf(stderr,"ERROR: Could not read the file\n");
    exit(1);
  }
  File f;
  f.data = buf;
  f.size = size;
  return f;
}

void print_board(int* board){

  for(int i=0; i<5; i++){
    for(int j=0; j<5; j++){
      printf("%2d ",board[i*5 + j]);
    }
    printf("\n");
  }
  
}

int who_wins(int** boards,int num_of_boards){
  int is_win;
  assert(MAX_NUM_OF_BOARD >= num_of_boards);
  static int winners[MAX_NUM_OF_BOARD] = {0};

  for(int board=0; board<num_of_boards; board++){
    if(winners[board]) continue;
    for(int row=0; row<5; row++){
      is_win = 1;
      for(int col=0; col<5; col++){
	if(boards[board][row*5 + col] != -1) is_win = 0;
      }
      if(is_win){
	winners[board] = 1;
	return board;
      }
    }

    for(int col=0; col<5; col++){
  	is_win = 1;
	for(int row=0; row<5; row++){
	  if(boards[board][row*5 + col] != -1) is_win = 0;
	}
	if(is_win){
	  winners[board] = 1;	
	  return board;
	}      
      }
    
  }
  return -1;
}

int calculate_sum_of_unchecked_num(int* board){
  int sum = 0;
  for(int i=0; i<25; i++) if(board[i] != -1) sum += board[i];
  return sum;
}

// Time complexity : O(sizeof_input * num_of_boards^2) ??
int calculate_final_score(int **boards,int num_of_boards,int *input,int sizeof_input){
  int winner_board = -1;
  int input_state;
  int sum=0;
  for(int i=0; i<sizeof_input; i++){
    for(int board=0; board<num_of_boards; board++){
      for(int num=0; num<25; num++){
	if(boards[board][num] == input[i]) boards[board][num] = -1;
      }
    }
    
    winner_board = who_wins(boards,num_of_boards);
    if(winner_board != -1) {
      sum = calculate_sum_of_unchecked_num(boards[winner_board]);
      input_state = input[i];
      return (sum * input_state);      
    }
    
  }
  
  return -1;
}

int calculate_final_score_of_last_winner(int **boards,int num_of_boards,int *input,int sizeof_input){
  int winner_board = -1;
  int input_state;
  int sum=0;
  for(int i=0; i<sizeof_input; i++){
    for(int board=0; board<num_of_boards; board++){
      for(int num=0; num<25; num++){
	if(boards[board][num] == input[i]) boards[board][num] = -1;
      }
    }
    
    winner_board = who_wins(boards,num_of_boards);
    while(winner_board != -1) {
      sum = calculate_sum_of_unchecked_num(boards[winner_board]);
      input_state = input[i];
      winner_board = who_wins(boards,num_of_boards);
    }
  }

  if(sum == 0) return -1;
  else return (sum * input_state);
}


int part1(const char* file_path){
  File input = Load_file(file_path);
  char* data = input.data;
  int* input_nums = malloc(sizeof(int) * MAX_GAME_LEN);
  int game_len = 0;
  int** boards = NULL;
  int num_of_board = 0;
  
  while(*data != '\n'){
    if(*data != ',') game_len++;
    data++;
  }

  assert(game_len <= MAX_GAME_LEN);
  data = input.data;

  game_len = 0;
  while(*data != '\n'){
    if(*data != ','){
      input_nums[game_len++] = atoi(data);
      data++;
    }
    data++;
  }

  SKIP_LINE(data);

  while(data < input.data + input.size){
    
    if(boards == NULL){
      boards = (int**)malloc(sizeof(int*) * 1);
      num_of_board = 1;
      *boards = (int*)malloc(sizeof(int) * 25);
    }
    else{
      boards = realloc(boards,sizeof(int*) * (num_of_board + 1));
      boards[num_of_board] = (int*)malloc(sizeof(int) * 25);
      num_of_board++;
    }

    for(int cur=0; cur<25; cur++){
      while(*data == '\n' || *data == ' ' ){
	assert(data < input.data + input.size);
	data++;
      }
      assert(data < input.data + input.size);
      boards[num_of_board - 1][cur] = atoi(data);
      while(*data != '\n' && *data != ' ' ){
	assert(data < input.data + input.size);
	data++;
      }
    }
    
    SKIP_LINE(data);
  }

  return calculate_final_score(boards,num_of_board,input_nums,game_len);
}


int part2(const char* file_path){
  File input = Load_file(file_path);
  char* data = input.data;
  int* input_nums = malloc(sizeof(int) * MAX_GAME_LEN);
  int game_len = 0;
  int** boards = NULL;
  int num_of_board = 0;
  
  while(*data != '\n'){
    if(*data != ',') game_len++;
    data++;
  }

  assert(game_len <= MAX_GAME_LEN);
  data = input.data;

  game_len = 0;
  while(*data != '\n'){
    if(*data != ','){
      input_nums[game_len++] = atoi(data);
      data++;
    }
    data++;
  }

  SKIP_LINE(data);

  while(data < input.data + input.size){
    
    if(boards == NULL){
      boards = (int**)malloc(sizeof(int*) * 1);
      num_of_board = 1;
      *boards = (int*)malloc(sizeof(int) * 25);
    }
    else{
      boards = realloc(boards,sizeof(int*) * (num_of_board + 1));
      boards[num_of_board] = (int*)malloc(sizeof(int) * 25);
      num_of_board++;
    }

    for(int cur=0; cur<25; cur++){
      while(*data == '\n' || *data == ' ' ){
	assert(data < input.data + input.size);
	data++;
      }
      assert(data < input.data + input.size);
      boards[num_of_board - 1][cur] = atoi(data);
      while(*data != '\n' && *data != ' ' ){
	assert(data < input.data + input.size);
	data++;
      }
    }
    
    SKIP_LINE(data);
  }

  return calculate_final_score_of_last_winner(boards,num_of_board,input_nums,game_len);
}


int main(void){
  int ans;
  ans = part1("./input.txt");
  assert(ans != -1 && "Nobody won????");
  printf("part1: %d\n",ans);
  ans = part2("./input.txt");
  assert(ans != -1 && "Nobody won????");
  printf("part2: %d\n",ans);
  return 0;
}
