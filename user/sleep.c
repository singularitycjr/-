#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("missing parameter 'time'\n");
    exit(1);
  }
  // atoi sys call guarantees return an integer
  int ticks = atoi(argv[1]);
  //printf("%d",ticks);
  if (ticks==0)
  {
    printf("parameter 'time' must be 'int' type,and must be positive\n");
  }
  int ret = sleep(ticks);
  exit(ret);
}