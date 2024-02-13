#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>
#include "kernel/fcntl.h"
#include <stdbool.h>

int main(){

  int fd = open("README.md", O_RDONLY);
  //while (true) {
    uniq(fd);
    
    //printf("Buffer address: %p\n", line);
    //printf("Line:           %s\n", line);
  //}
  return 0;

}

