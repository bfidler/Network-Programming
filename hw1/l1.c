#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // read() and write() prototypes are here

//------------------------------------------------------------------
// Sample layer 1 implementation based on a unix pipe.
//

int readfd,writefd;
int generror=0;

void init_l1(void) {
  static int first=1;
  int fds[2];

  if (! first) {
    close(readfd);
    close(writefd);
  } else {
    first=0;
  }

  if (pipe(fds)==-1) {
    printf("ERROR: Can't create pipe \n");
    exit(1);
  }
  readfd = fds[0];
  writefd = fds[1];
}



// l1_read read from the pipe unless generror is 1
int l1_read( char *b) {
  if (generror==2) {
    return(-1);
  } else {
    if (read(readfd,b,1)!=1) 
      return(-1);
    else
      return(1);
  }
}

// l1_write writes to the pipe unless generror is 2
int l1_write(char b) {
  if (generror==1) {
    return(-1);
  } else {
    if (write(writefd,&b,1)!=1) 
      return(-1);
    else
      return(1);
  }
}
