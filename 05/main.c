#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#define MAX_MAP_SIZE 1000
#define SKIP_DIGIT(ptr) while(*ptr >= '0' && *ptr <= '9') ptr++ 
#define NEXT_DIGIT(ptr) while(!((*ptr >= '0') && (*ptr <= '9'))) ptr++

int cords[MAX_MAP_SIZE][MAX_MAP_SIZE] = {0};
int delay = 0;

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

void Dump_map(){
  for(int i=0; i<MAX_MAP_SIZE; i++){
    for(int j=0; j<MAX_MAP_SIZE; j++){
      printf("%d ",cords[i][j]);
    }
    printf("\n");
  }
}

int part1(const char* file_path){
  memset(cords,0,sizeof(int) * MAX_MAP_SIZE * MAX_MAP_SIZE);
  File input = Load_file(file_path);
  printf("File size of %s: %d\n",file_path,input.size);
  char* data = input.data;

  int x,y,x1,y1,x2,y2;
  int cnt;
    
  while(data < input.data + input.size - 1){
    NEXT_DIGIT(data);
    x = atoi(data);
    SKIP_DIGIT(data);
    NEXT_DIGIT(data);
    y = atoi(data);
    SKIP_DIGIT(data);
    NEXT_DIGIT(data);
    //    printf("x1 = %d  y1 = %d  |  ",x,y);
    assert(x < MAX_MAP_SIZE && y < MAX_MAP_SIZE);
    x1 = x;
    y1 = y;
    
    x = atoi(data);
    SKIP_DIGIT(data);
    NEXT_DIGIT(data);
    y = atoi(data);
    SKIP_DIGIT(data);
    //    printf("x2 = %d  y2 = %d\n",x,y);
    assert(x < MAX_MAP_SIZE && y < MAX_MAP_SIZE);
    x2 = x;
    y2 = y;

    if(x1 == x2){
      x = x1;
      int y_min = (y1 <= y2)?y1:y2;
      int y_max = (y1 > y2)?y1:y2;
      for(int y=y_min; y<=y_max; y++){
	cords[y][x]++;
      }
    }
    else if(y1 == y2){
      y = y1;
      int x_min = (x1 <= x2)?x1:x2;
      int x_max = (x1 >  x2)?x1:x2;
      for(int x=x_min; x<=x_max; x++){
	cords[y][x]++;
      }
    }

    // Calculating the values greater than 2 in the cordinates
    cnt = 0;
    for(int y=0; y<MAX_MAP_SIZE; y++){
      for(int x=0; x<MAX_MAP_SIZE; x++){
	if(cords[y][x] >= 2){
	  cnt++;
	}
      }
    }
  }
  //Dump_map();
  return cnt;
}

int part2(const char* file_path){
  memset(cords,0,sizeof(int) * MAX_MAP_SIZE * MAX_MAP_SIZE);
  File input = Load_file(file_path);
  printf("File size of file %s: %d\n",file_path,input.size);
  char* data = input.data;
  int x_min;
  int x_max;
  int y_min;
  int y_max;

  int x,y,x1,y1,x2,y2;
  int cnt;
    
  while(data < input.data + input.size - 1){
    NEXT_DIGIT(data);
    x = atoi(data);
    SKIP_DIGIT(data);
    NEXT_DIGIT(data);
    y = atoi(data);
    SKIP_DIGIT(data);
    NEXT_DIGIT(data);
    //    printf("x1 = %d  y1 = %d  |  ",x,y);
    assert(x < MAX_MAP_SIZE && y < MAX_MAP_SIZE);
    x1 = x;
    y1 = y;
    
    x = atoi(data);
    SKIP_DIGIT(data);
    NEXT_DIGIT(data);
    y = atoi(data);
    SKIP_DIGIT(data);
    //    printf("x2 = %d  y2 = %d\n",x,y);
    assert(x < MAX_MAP_SIZE && y < MAX_MAP_SIZE);
    x2 = x;
    y2 = y;

    x_min = (x1 <= x2)?x1:x2;
    x_max = (x1 >  x2)?x1:x2;
    y_min = (y1 <= y2)?y1:y2;
    y_max = (y1 > y2)?y1:y2;
    //printf("x:%d X:%d y:%d Y:%d\n",x_min,x_max,y_min,y_max);

    if(x1 == x2){
      x = x1;
      for(int y=y_min; y<=y_max; y++){
	cords[y][x]++;
      }
      //      Dump_map();
      //printf("\n----------\n");
      //sleep(delay);
    }
    
    else if(y1 == y2){
      y = y1;
      for(int x=x_min; x<=x_max; x++){
	cords[y][x]++;
      }
      //      Dump_map();
      //      printf("\n----------\n");      
      //sleep(delay);
    }
    
    else if((x_max - x_min) == (y_max - y_min)){

      if(x1 < x2 && y1 < y2){
	while(x1 <= x2 && y1 <= y2){
	  cords[y1][x1]++;
	  x1++,y1++;
	}
      }

      else if(x1 > x2 && y1 < y2){
	while(x1 >= x2 && y1 <= y2){
	  cords[y1][x1]++;
	  x1--,y1++;
	}
      }

      else if(x1 < x2 && y1 > y2){
	while(x1 <= x2 && y1 >= y2){
	  cords[y1][x1]++;
	  x1++,y1--;
	}
      }

      else if(x1 > x2 && y1 > y2){
	while(x1 >= x2 && y1 >= y2){
	  cords[y1][x1]++;
	  x1--,y1--;
	}
      }
      //Dump_map();
      //      printf("\n----------\n");
      //sleep(delay);
    }

    // Calculating the values greater than 2 in the cordinates
    cnt = 0;
    for(int y=0; y<MAX_MAP_SIZE; y++){
      for(int x=0; x<MAX_MAP_SIZE; x++){
	if(cords[y][x] >= 2){
	  cnt++;
	}
      }
    }
  }
  //  Dump_map();
  return cnt;
}

int main(void){
  printf("part1: %d\n",part1("./input.txt"));
  printf("part2: %d\n",part2("./input.txt"));
  return 0;
}
