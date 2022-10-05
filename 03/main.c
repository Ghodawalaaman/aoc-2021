#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


typedef struct{
  char* data;
  int size;
} File;

int count_lines(char* data,int size){
  int lines = 0;

  for(int i=0; i<size; i++){
    if(data[i] == '\n') lines++;
  }
  return lines;
}

char* read_line_from_file(FILE* file){
  size_t len = 0;
  char* buf;
  int status = getline(&buf,&len,file);
  if(status == -1) return NULL;
  else return buf;
}

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

int part1(const char* file_path){
  FILE* file     = fopen(file_path,"r");
  char* line     = read_line_from_file(file);
  int   len      = strlen(line) - 1;
  int*  arr      = calloc(1,sizeof(int) * len);
  char* ans      = malloc(sizeof(char) * (len+1));
  ans[len] = '\0';

  int gamma_rate,epsilon_rate;
  
  while(1){
    for(int i=0; i<len; i++){
      if(line[i] == '1') arr[i]++;
      else arr[i]--;
    }
    line = read_line_from_file(file);
    if(line == NULL) break;
  }

  for(int i=0; i<len; i++){
    if(arr[i] > 0) sprintf(ans+i,"1");
    else sprintf(ans+i,"0");
  }
  gamma_rate = strtol(ans,NULL,2);

  for(int i=0; i<len; i++){
    if(ans[i] == '0') ans[i] = '1';
    else ans[i] = '0';
  }
  epsilon_rate = strtol(ans,NULL,2);

  return epsilon_rate * gamma_rate;
}

int part2(const char* file_path){
  File input = Load_file(file_path);
  int lines = count_lines(input.data,input.size);
  int  bits  = 0;
  char* tmp  = input.data;
  while(*tmp++ != '\n') bits++;
  char* ans  = malloc(sizeof(char) * (bits+1));
  ans[bits] = '\0';
  int* out = calloc(1,sizeof(int) * lines);
  int ones = 0;
  int cnt = 0;
  int o2;
  int co2;
  
  // The oxygen rate
  for(int bit=0; bit<bits; bit++){
    for(int line=0; line<lines; line++){
      if(!out[line]){
	if(*(input.data + (bits+1)*line + bit) == '1') ones++;
	else ones--;
      }
    }
    if(ones >= 0) ans[bit] = '1';
    else ans[bit] = '0';

    for(int line=0; line<lines; line++){
      if(*(input.data + (bits+1)*line + bit) != ans[bit]) out[line] = 1;
    }
    ones = 0;
  }
  o2 = strtol(ans,NULL,2);

  ones = 0;
  memset(out,0,sizeof(int)*lines);
  // The CO2 Scrubber rate
  for(int bit=0; bit<bits; bit++){
    for(int line=0; line<lines; line++){
      if(!out[line]){
	if(*(input.data + (bits+1)*line + bit) == '1') ones++;
	else ones--;
      }
    }
    if(ones < 0) ans[bit] = '1';
    else ans[bit] = '0';

    for(int line=0; line<lines; line++){
      if(*(input.data + (bits+1)*line + bit) != ans[bit]) out[line] = 1;
    }

    // Check for the stoping condition
    cnt = 0;
    int index = 0;
    for(int i=0; i<lines; i++){
      if(!out[i]) cnt++,index = i;
    }
    if(cnt <= 1){
      for(int i=0; i<bits; i++) ans[i] = *(input.data + (bits+1)*index + i);
      break;
    }

    ones = 0;
  }
  co2 = strtol(ans,NULL,2);

  
  return o2 * co2;
}

int main(void){
  printf("part1: %d\n",part1("input.txt"));
  printf("part2: %d\n",part2("input.txt"));
  return 0;
}
