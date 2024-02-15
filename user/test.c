#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>
#include "kernel/fcntl.h"
#include <stdbool.h>

int main(int argc, char *argv[]){

  int fd = open(argv[1], O_RDONLY);
  uniq(fd, argv);;
  return 0;

}

