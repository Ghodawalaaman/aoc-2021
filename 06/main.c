#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_DAYS  256
#define MAX_TIMER 8

// Some global variable

static int fishes_footprint[MAX_TIMER+1][MAX_DAYS+1]; // I really can't come up with a good name.

typedef struct{
  int timer;
  int remaining_days;
} Fish;

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

void display_fish_info(Fish *fishes,int num_of_fishes){
  printf("\n----------------------------------------\n");
  for(int i=0; i<num_of_fishes; i++){
    printf("Fish timer: %d    Reamining days: %d\n",fishes[i].timer,fishes[i].remaining_days);
  }
  printf("----------------------------------------\n");
}

int count_fish(Fish* fishes,int num_of_fishes){

  // The dumb way to count the fish. This approach will cosume all of your computers memory and CPU if the number of days is large. 

  if(num_of_fishes <= 0) return 0;

  int result = 0;
  for(int fish=0; fish<num_of_fishes; fish++){
    int have_child = (fishes[fish].remaining_days - fishes[fish].timer - 1);
    if(have_child < 0){
      result += 1;
      continue;
    }

    int childs = ((fishes[fish].remaining_days - fishes[fish].timer - 1) / 7) + 1;

    Fish *child_fishes = malloc(sizeof(Fish) * childs);
    if(child_fishes == NULL ){
      fprintf(stderr,"malloc failure\n");
      exit(1);
    }
    int num_of_child_fishes = 0;
    int days_left = (fishes[fish].remaining_days - fishes[fish].timer - 1);
    child_fishes[num_of_child_fishes].timer = 8;
    child_fishes[num_of_child_fishes].remaining_days = days_left;

    num_of_child_fishes++;
    
    while(days_left >= 7){
      days_left -= 7;
      child_fishes[num_of_child_fishes].remaining_days = days_left;
      child_fishes[num_of_child_fishes].timer = 8;
      num_of_child_fishes++;
    }

    result += (1 + count_fish(child_fishes,num_of_child_fishes));
    free(child_fishes);
  }
  return result;
}

int part1(const char* file_path){
  File file = Load_file(file_path);
  int num_of_fishes = file.size / 2;
  Fish *fishes = (Fish*) malloc(sizeof(Fish) * num_of_fishes);
  char* data = file.data;

  for(int i=0; 2*i < file.size; i++){
    fishes[i].timer = (int)*data - 48;
    fishes[i].remaining_days = 80;
    data += 2;
  }
  
  //display_fish_info(fishes,num_of_fishes);

  int total_fish = count_fish(fishes,num_of_fishes);
  free(fishes);
  return total_fish;
}

void print_fishes_at_timestamp(int day){
  printf("\n---------------------------------------------\n");
  
  for(int timer=0; timer<=MAX_TIMER; timer++){
    printf("NUM OF FISHES AT DAY %2d WITH TIMER %2d IS %2d\n",day,timer,fishes_footprint[timer][day]);
  }

  printf("---------------------------------------------\n");
}

int count_fish2(Fish* fishes,int num_of_fishes,int days){
  memset(fishes_footprint,0, sizeof(int) * (MAX_TIMER+1) * (MAX_DAYS+1));

  int cday = 0;
  for(int fish=0; fish<num_of_fishes; fish++){
    fishes_footprint[fishes[fish].timer][cday]++;
  }
  
  while(cday <= days){
    for(int timer=0; timer<=MAX_TIMER; timer++){
      int num_of_parents = fishes_footprint[timer][cday];
      int days_left = days - cday - timer - 1;
      while(days_left >= 7){
	int spawn_day = days - days_left;
	fishes_footprint[8][spawn_day] += num_of_parents;
	days_left -= 7;
      }
    }
    cday++;
  }
  
  print_fishes_at_timestamp(2);
  return 0;
}

int part2(const char* file_path){
  File file = Load_file(file_path);
  int num_of_fishes = file.size / 2;
  Fish *fishes = (Fish*) malloc(sizeof(Fish) * num_of_fishes);
  char* data = file.data;

  for(int i=0; 2*i < file.size; i++){
    fishes[i].timer = (int)*data - 48;
    fishes[i].remaining_days = 80;
    data += 2;
  }
  
  //display_fish_info(fishes,num_of_fishes);

  int total_fish = count_fish2(fishes,num_of_fishes,256);
  free(fishes);
  return total_fish;

}

int main(void){
  printf("part1: %d\n",part1("./sample.txt"));
  printf("part2: %d\n",part2("./sample.txt"));
  return 0;
}
