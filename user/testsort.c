#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>
#include "kernel/fcntl.h"
#include <stdbool.h>

int main(int argc, char *argv[]) {
	char * list[] = {"hello", "hello", "apple", "catch", "zebra"};

	//bubble_sort(char* list[], int length) {
	bubble_sort(list, 5);
	printf("sorted\n");
	return 0;
}
