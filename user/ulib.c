#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include <stdbool.h>

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

int
gets(char *buf, int max)
{
  return fgets(buf, max, 0);
}

int
fgets(char *buf, int max, int file_des){
  int i = 0, cc;
  char c;

  for(i=0; i+1<max;){
	cc = read(file_des, &c, 1);
	if(cc<1)
	   break;
	buf[i++] = c;
	if(c == '\n' || c== '\r')
	   break;
  }

  buf[i] = '\0';
  return i;
}

int getline(char **lineptr, int *sz, int fd){
   if(*sz == 0 && *lineptr ==0){
	*sz = 1024;
	*lineptr = malloc(*sz);
   }

   char *buf = *lineptr;
   int total = 0;
   int result = 0;
  
   while(buf[total-1] != '\n'){
	result = fgets(buf + total, *sz - total, fd);
	total += result;

	if(result == 0){
	   return total;
	}

	else if (result == -1){
	   return total;
	}

	else{
	   *sz = *sz * 2;
	   char *new_buf = malloc(*sz);
	   memcpy(new_buf, *lineptr, *sz/2);
	   free(*lineptr);
	   *lineptr = new_buf;
	   buf = *lineptr;
	}
	
   }

   return total;
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


void
uniq(int fd){

  int sz = 10; int count = 0;
  char *line = malloc(sz);
  char *lines[1024];

  while (true) {
    if (getline(&line, &sz, fd) <= 0) {
      break;
    }

    lines[count] = line;
    count++;
  }  


  printf("%s\n", lines[0]);
  bubble_sort(lines, count);
  printf("%s\n", lines[0]);

  int i = 0; int current = 0;
  while(strcmp(lines[i],"\n")){
	if(current == 0 || i == 0){
	   current++;
	}

	else if(!strcmp(lines[i], lines[i-1])){
	   current++;
	}

	else{
	   //printf("%d %s", current, lines[i]);
	   current = 1;
	}

	printf("%d %s\n", current, lines[i]);
	i++;

  }


}

void
swap(char * list[], int i, int j) {
	char * temp = list[i];
	list[i] = list[j];
	list[j] = temp;
}

void
bubble_sort(char *list[], int length) {
	for (int i = 0; i < length; i++) {
		for (int j = i + 1; j < length; j++) {
			if (strcmp(list[i], list[j]) > 0) {
				swap(list, i, j);
				//printf("swapping %s with %s\n", list[i], list[j]);
			}
		}
		//printf("list[%d] is %s\n", i, list[i]);
	}
}
