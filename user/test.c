#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>
#include "kernel/fcntl.h"
#include <stdbool.h>

int main(int argc, char *argv[]){

  // int fd = open(argv[1], O_RDONLY);
  // printf("opened argv[1]\n");
  // printf("fd: %d\n", fd);
  uniq(argv);
  return 0;

}

