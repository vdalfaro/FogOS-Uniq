#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include <stdbool.h>
#include <stddef.h>

//
// wrapper so that it's OK if main() does not call exit().
//
void
_main()
{
  extern int main();
  main();
  exit(0);
}

char*
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
    cdst[i] = c;
  }
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  fgets(0, buf, max);
  return buf;
}

int
fgets(int fd, char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(fd, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return i;
}

int
getline(char **lineptr, int *n, int fd)
{
  if (*lineptr == 0 && *n == 0) {
    *n = 128;
    *lineptr = malloc(*n);
  }

  char *buf = *lineptr;
  uint total_read = 0;
  while (1) {
    int read_sz = fgets(fd, buf + total_read, *n - total_read);
    if (read_sz == 0) {
      return total_read;
    } else if (read_sz == -1) {
      // error
      return -1;
    }

    total_read += read_sz;
    if (buf[total_read - 1] == '\n') {
      break;
    }

    uint new_n = *n * 2;
    char *new_buf = malloc(new_n);
    memcpy(new_buf, buf, *n);
    free(buf);

    buf = new_buf;

    *n = new_n;
    *lineptr = buf;
  }

  return total_read;
}

int
stat(const char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, const void *vsrc, int n)
{
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  if (src > dst) {
    while(n-- > 0)
      *dst++ = *src++;
  } else {
    dst += n;
    src += n;
    while(n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}

int
memcmp(const void *s1, const void *s2, uint n)
{
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
    if (*p1 != *p2) {
      return *p1 - *p2;
    }
    p1++;
    p2++;
  }
  return 0;
}

void *
memcpy(void *dst, const void *src, uint n)
{
  return memmove(dst, src, n);
}

/*
* writes a line word by word to a given file
*
* @param line to write
* @param fd file descriptor to write to
*/
void *
writewords(char *line, int fd){
	// first char we check is the start of the line
  char *character = &line[0];
  while (*character != '\0') { // while we aren't at the end
   	if (*character == ' ') { // if we reach the end of a word
      fprintf(fd, "\n");
    }
    else {
      fprintf(fd, "%c", *character);
    }
    character++;
  }
 
	return 0;
}

/*
* prints out the uniq instances from a provided sorted
* char* array
*
* @param count max num of lines or words
* @param lines array of lines or words
* @param cflag boolean for the -c flag
*/
void print_uniq(int count, char *lines[], bool cflag) {
  int curr = 0; // keeps track of which line we're at
  int instances = 0; // keeps track of how many instances
  while(curr < count) { // while we still have stuff to read
    if (curr == 0) {
       instances++; // everything has at least one instance 
    }
    // skip null terms and line ends
    else if ((strcmp(lines[curr], "\0") == 0) || (strcmp(lines[curr], "\n") == 0)) {
       continue;
    }
		else if(strcmp(lines[curr], lines[curr + 1]) == 0){
      instances++;
		}
		// if we have hit the last uniq instance, print it out
    else {
      if (cflag) { // with count
        printf("%d %s", instances, lines[curr]);
      }
      else { // without count
        printf("%s", lines[curr]);
      }
      instances = 1;
    }

    curr++;
  }
}

/*
* creates a char * of all the lines from a given file and
* prints out all the unique lines
*
* @param fd file descriptor of file to read
* @param cflag boolean for the -c flag
*/
void lines(int fd, bool cflag) {
	int sz = 20; // getline will resize if necessary
	int count = 0; // num of lines
  char *lines[1024];

	while (true) {
		char *line = malloc(sz);
    if (getline(&line, &sz, fd) <= 0) {
      break;
    }

    lines[count] = line;
    count++;
  }	
	
	bubble_sort(lines, count); // sort all the lines
  print_uniq(count, lines, cflag); // print uniq instances
}

/*
* creates a char * of all the words from a given file and
* prints out all the unique words
*
* @param fd file descriptor of file to read
* @param cflag boolean for the -c flag
*/
void words(int fd, bool cflag){
	int sz = 20; // getline will resize if necessary
	int count = 0; // num of lines
  // file we are writing the words to
	int write = open("/temp.txt", O_CREATE | O_WRONLY);

	while (true) {
		char *line = malloc(sz);
    if (getline(&line, &sz, fd) <= 0) {
      break;
    }

    writewords(line, write);
    count++;
  }
  close(write);
	// have to close & reopen file because xv6 hates us

	// file to now read the words from
	int read = open("/temp.txt", O_RDONLY);
  int wordcount = 0; // num words
  char *words[1024];
  int newsize = 10;
  
  while (true) {
		char *word = malloc(newsize);
    if(getline(&word, &newsize, read) <= 0) {
      break;
    }
      
    words[wordcount] = word;
    wordcount++;
  }

  close(read);
	bubble_sort(words, wordcount); // sort all the words
  print_uniq(wordcount, words, cflag); // print unique words
}

/*
* creates a char * of all the lines from a given file and
* prints out all the unique lines
*
* @param argv array of args to check for flags
* @param fd file descriptor of file to read
*/
void
uniq(char *argv[], int fd) {
	int j = 0; 

  //FINDING PROPER FLAGS
  while (argv[j] != NULL) {
		if (!strcmp(argv[j], "-c")) {
	  	lines(fd, true);
			return;
		}

		else if (!strcmp(argv[j], "-wc")) {
			words(fd, true);
			return;
		}
	
		else if (!strcmp(argv[j], "-w")) {
	  	words(fd, false);
			return;
		}
		j++;
	}
	
	lines(fd, false);
}

/*
* helper function for sort that swaps two
* values from an array
*
* @param array to swap in
* @param i one index to swap
* @param j other index to swap
*/
void
swap(char * list[], int i, int j) {
	char * temp = list[i];
	list[i] = list[j];
	list[j] = temp;
}

/*
* bubble sorts an array of char *
*
* @param array to sort
* @param length of array
*/
void
bubble_sort(char *list[], int length) {
	for (int i = 0; i < length; i++) {
		for (int j = i + 1; j < length; j++) {
			if (strcmp(list[i], list[j]) > 0) {
				swap(list, i, j);
			}
		}
	}
}
