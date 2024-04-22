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
  char c; int first_instance = 0;

  for (i = 0; i + 1 < max; ) {
    cc = read(fd, &c, 1);
    if (cc < 1)
      break;
		if ((c != ' '  && c != '\t') || first_instance == 1) {
			 first_instance = 1;
		   buf[i++] = c;	
		}
	  if (c == '\n' || c == '\r') {
	  	break;
	  }
	  else if (c == '\t') { // we want to remove tabs
	  	i--;
	  	break;
	  }
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
    if (buf[total_read - 1] == '\n' || buf[total_read - 1] == '\t') {
      return total_read;
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

