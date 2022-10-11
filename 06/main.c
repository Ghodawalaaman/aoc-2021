#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

#define MAX_DAYS  256
#define MAX_TIMER 8

typedef __int128_t ll;

// Some global variable

static ll fishes_spawn_cnt[MAX_DAYS]; // I really can't come up with a good name.

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

void print_fishes(){
  printf("\n---------------------------------------------\n");

  for(int day=0; day<MAX_DAYS; day++)
  printf("num of fishes on day %d is %llu\n",day+1,fishes_spawn_cnt[day]);
  
  printf("---------------------------------------------\n");
}

ll count_fish2(Fish* fishes,int num_of_fishes,int days){
  memset(fishes_spawn_cnt,0, sizeof(ll) * (MAX_DAYS));
  int cday = 0;

  // Counting the number of child fishes produced by the parents.
  for(int cfish=0; cfish<num_of_fishes; cfish++){
    cday = fishes[cfish].timer;
    fishes_spawn_cnt[cday]++;

    while(cday < days){
      cday += 7;
      if(cday >= days) break;
      fishes_spawn_cnt[cday]++;
    }
  }

  // Counting the number of child fishes produce by the child fishes.
  for(int day=0; day<days; day++){
    cday = day;
    ll num_of_parents = fishes_spawn_cnt[day];
    cday += (9 - 0);
    if(cday >= days) break;
    fishes_spawn_cnt[cday] += num_of_parents;

    while(cday < days){
      cday += 7;
      if(cday >= days) break;
      fishes_spawn_cnt[cday] += num_of_parents;
    }
    
  }

  print_fishes();
  ll sum = 0;
  ll psum = sum;
  // Calculate the total number of fishes
  for(int day=0; day<days; day++){
    psum = sum;
    assert(sum >= psum && "Interger overflow!!");
    sum += fishes_spawn_cnt[day];
  }

  return sum + (ll)num_of_fishes;
}

ll part1(const char* file_path){
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

  ll total_fish = count_fish(fishes,num_of_fishes);
  free(fishes);
  return total_fish;
}

ll part2(const char* file_path){
  File file = Load_file(file_path);
  int num_of_fishes = file.size / 2;
  Fish *fishes = (Fish*) malloc(sizeof(Fish) * num_of_fishes);
  char* data = file.data;
  int days = 256;
  
  for(int i=0; 2*i < file.size; i++){
    fishes[i].timer = (int)*data - 48;
    fishes[i].remaining_days = days;
    data += 2;
  }
  __int128_t aman;
  //display_fish_info(fishes,num_of_fishes);

  ll total_fish = count_fish2(fishes,num_of_fishes,days);
  free(fishes);
  return total_fish;

}

int main(void){
  printf("part1: %llu\n",part1("./input.txt"));
  printf("part2: %llu\n",part2("./input.txt"));
  return 0;
}
