#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

char* read_line_from_file(FILE* file){
  size_t len = 0;
  char* buf;
  int status = getline(&buf,&len,file);
  if(status == -1) return NULL;
  else return buf;
}

void part1(const char* file_path){

  FILE* file = fopen(file_path,"r");
  if(file == NULL){
    fprintf(stderr,"ERROR: Could not open the file");
    exit(1);
  }
  char* buf = NULL;
  int run = 1;
  int Measurement = INT_MAX;
  int PrevMeasurement = INT_MAX;

  int cnt = 0;
  
  while(run){
    buf = read_line_from_file(file);
    if(buf == NULL){
      printf("The Depth increased %d times!!\n",cnt);
      return;
    }
    PrevMeasurement = Measurement;
    Measurement = atoi(buf);
    if(Measurement > PrevMeasurement) cnt++;
    free(buf);
  }

}

void part2(const char* file_path){

  FILE* file = fopen(file_path,"r");
  if(file == NULL){
    fprintf(stderr,"ERROR: Could not open the file");
    exit(1);
  }
  char* buf = NULL;
  int run = 1;
  int Measurement = INT_MAX;
  int PrevMeasurement = INT_MAX;
  int PrevPrevMeasurement = INT_MAX;
  int csum=INT_MAX,psum=INT_MAX;
  int cnt = 0;
  
  while(run){
    buf = read_line_from_file(file);
    if(buf == NULL){
      printf("The Depth increased %d times!!\n",cnt);
      return;
    }
    PrevPrevMeasurement = PrevMeasurement;
    PrevMeasurement = Measurement;
    Measurement = atoi(buf);

    if(PrevPrevMeasurement == INT_MAX ||
       PrevMeasurement     == INT_MAX ||
       Measurement         == INT_MAX)
      {
	continue;
      }

    psum = csum;
    csum = PrevPrevMeasurement + PrevMeasurement + Measurement;
    if(psum == INT_MAX) continue;
    if(csum > psum) cnt++;
    free(buf);
  }
}

int main(void){
  part1("./input.txt");
  part2("./input.txt");
  return 0;
}
