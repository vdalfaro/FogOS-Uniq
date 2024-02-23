#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>
#include "kernel/fcntl.h"
#include <stdbool.h>

int main(int argc, char *argv[]){

	if (argc < 2) {
		printf("usage: uniq filename [-w] [-c] [-wc]\n");
		return -1;
	}

  int fd = open(argv[1], O_RDONLY);
  if (fd < 1) {
  	printf("err opening file\n");
  	return -1;
  }
  uniq(argv, fd);
  return 0;

}

