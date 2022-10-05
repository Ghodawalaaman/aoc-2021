#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

char* read_line_from_file(FILE* file){
  size_t len = 0;
  char* buf;
  int status = getline(&buf,&len,file);
  if(status == -1) return NULL;
  else return buf;
}

char** chop_by_delim(char* str,char del){
  int len        = strlen(str);
  int target_loc = 0;
  char** token   = (char**) malloc(sizeof(char*) * 2);
  
  while(target_loc < len && str[target_loc] != del) target_loc++;
  
  int size1 = target_loc;
  int size2 = len - target_loc;
  
  if(size1 <= len){
    token[0] = (char*) malloc(sizeof(char) * (size1 + 1));
    token[1] = (char*) malloc(sizeof(char) * (size2 + 1));
    for(int i=0;     i<size1; i++) token[0][i]       = str[i];
    for(int i=size1; i<len  ; i++) token[1][i-size1] = str[i];
    token[0][size1] = '\0';
    token[1][size2] = '\0';
    if(size1 == len - 1) token[1][0] = '\0';
  }

  return token;
}

int part1(const char* path_name){
  FILE* file     = fopen(path_name,"r");
  char* buf      = NULL;
  int horizontal = 0;
  int depth      = 0;
  
  while(1){
    buf = read_line_from_file(file);
    if(buf == NULL){
      return horizontal * depth;
    }

    char** tokens = chop_by_delim(buf,' ');
    int Change = atoi(tokens[1]);
    
    if     (strcmp(tokens[0],"forward") == 0) horizontal += Change;
    else if(strcmp(tokens[0],"up")      == 0) depth      -= Change;
    else if(strcmp(tokens[0],"down")    == 0) depth      += Change;
    else assert("Unreachable!");

    free(tokens[0]);
    free(tokens[1]);
    free(tokens);
    free(buf);
  }

}

int part2(const char* path_name){
  FILE* file     = fopen(path_name,"r");
  char* buf      = NULL;
  int horizontal = 0;
  int depth      = 0;
  int aim        = 0;
  
  while(1){
    buf = read_line_from_file(file);
    if(buf == NULL){
      return horizontal * depth;
    }

    char** tokens = chop_by_delim(buf,' ');
    int Change = atoi(tokens[1]);
    
    if     (strcmp(tokens[0],"forward") == 0) horizontal += Change, depth += Change * aim;
    else if(strcmp(tokens[0],"up")      == 0) aim        -= Change;
    else if(strcmp(tokens[0],"down")    == 0) aim        += Change;
    else assert("Unreachable!");

    free(tokens[0]);
    free(tokens[1]);
    free(tokens);
    free(buf);
  }

}

int main(void){
  printf("part1 :%d\n",part1("./input.txt"));
  printf("part2 :%d\n",part2("./input.txt"));
  return 0;
}
