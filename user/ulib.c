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


//TODO; SPLIT wc/c follows
// -- split needs to use strchr in theory to find the first index of ' '
// -- write word by word to another file and then use that file for the wc
//TODO: SIZE: fix
//GENERALIZE: ARGV

void
uniq(int fd, char *argv[]){
  int sz = 10; int count = 0;
  char *line = malloc(sz); int j = 0;
  char *lines[1024]; bool cflag = false;
  //bool wflag = false;
  // bool wflag = false; bool wcflag = false;

  while(argv[j] != NULL){
		if(!strcmp(argv[j], "-c")){ // || strcmp -wc
	  	cflag = true;
		}
		/*else if(!strcmp(argv[j], "-w")){
		else if(!strcmp(argv[j], "-w")){
	  	wflag = true;
		}*/

		/*else if(!strcmp(arg[j], "-wc")){
	   	wcflag = true;
		} */

		j++;
  }
	
  

  while (true) {
    if (getline(&line, &sz, fd) <= 0) {
      break;
    }

		//THIS CAN BE A FUNCTION
	  char *character = &line[0];
		while(*character != '\0'){
			if(!strcmp(character, " ")){
				printf("\n");
			}

			else{
				printf("%c", *character);
			}
			
			character++;
		}

    lines[count] = line;
    count++;
  }
  
  bubble_sort(lines, count);
  
	//THIS CAN BE A FUNCTIOn
  /*int i = 0; int current = 0;
  if (wflag) {
  	// would wflag use fgets because we want to split it
  	// at the \n and \r?
  	// or use strchr and find the first instance of a " " and 
  	// split it

		// instead of implementing split:
		// 	have a command so that we can do a 
		//  cat file & sort
		//  and have what the cat produced go into a file and then
		//  have a program that finds all the unique instances of a file
		//  and put that into a file
		//  and then have that be the input for uniq (pipe it into that?)
		//  so then uniq doesn't do just lines but can do words
		//  so we need to split the getline & the printing below.
		for (int i = 0; i < count; i++) { // loop over each line
			// for (int j = 0; line[i][j] != ' '; j ++) {
			// 	
			// }
		}
  }*/

  //printf("%s\n", lines[0]);
  /*for (int i = 0; i < count; i++) {
     printf("line %d is %s\n", i, lines[i]);
  }*/
  bubble_sort(lines, count);
  /*for (int i = 0; i < count; i++) {
     printf("line %d is %s\n", i, lines[i])
  */
  
  int i = 0; // keeps track of which line we're at
  int current = 0; // keeps track of how many instances?
  while(i < count){

		if(i == 0){
		   current++;
		}
		else if((strcmp(lines[i], "\0") == 0) || (strcmp(lines[i], "\n") == 0)) {
			 // current = 1;
		   continue;
		}
		else if(strcmp(lines[i], lines[i + 1]) == 0){
			 // printf("adding 1 to %d for %s\n", current, lines[i]);
		   current++;
		}

		else{
			if(cflag){
				//printf("%d %s", current, lines[i]);
		  }
		  else{
				//printf("%s", lines[i]);
		  }
		  current = 1;
		}	

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
