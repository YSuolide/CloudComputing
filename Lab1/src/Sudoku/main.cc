#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include<sys/wait.h>

#include "sudoku.h"

int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main(int argc, char* argv[])
{
  init_neighbors();

  char puzzle[128];
  int total_solved = 0;
  int total = 0;
  bool (*solve)(int) = solve_sudoku_dancing_links;

  int64_t start = now();
  int pid = fork();

  if(pid > 0){
    FILE* fp1 = fopen(argv[1], "r");
    while (fgets(puzzle, sizeof puzzle, fp1) != NULL && total <= 499) {
      if (strlen(puzzle) >= N) {
        ++total;
        input(puzzle);
        if (solve(0)) {
          ++total_solved;
          if (!solved())
            assert(0);
        }
        else {
          printf("No: %s", puzzle);
        }
      }
    }

    wait(NULL);
    int64_t end = now();
    double sec = (end-start)/1000000.0;
    printf("%f sec %f ms each %d\n", sec, 1000*sec/total, total_solved);
    printf("process 1 %d %d", total, total_solved);
  }
  else if(pid == 0){
    FILE* fp2 = fopen(argv[1], "r");
    fseek(fp2, 500*128, 0);
    while (fgets(puzzle, sizeof puzzle, fp2) != NULL && total <= 499) {
      if (strlen(puzzle) >= N) {
        ++total;
        input(puzzle);
        if (solve(0)) {
          ++total_solved;
          if (!solved())
            assert(0);
        }
        else {
          printf("No: %s", puzzle);
        }
      }
    }
    printf("process 1 %d %d", total, total_solved);
  }


  return 0;
}

