// This code tests layer 3
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // read() and write() prototypes are here
#include <string.h>
#include "l1.h"

//------------------------------------------------------------------
// layer 2 implementation

// l2_write sends a single byte that indicates the size of the chunk
//  (values 0 - 10 are legal)
// followed by the data.

int l2_write(char *chunk, int len) {

  int i;

  if (generror==1) return(-1);

  if (len<0 || len>10) {
    fprintf(stderr,"Error: l2_write called with invalid len\n");
    return(-1);
  }

  if (l1_write((char) len)!=1) {
    fprintf(stderr,"Error: l2_write got an error from l1_write\n");
    return(-1);
  }

  for (i=0;i<len;i++) {
    if (l1_write(chunk[i])!=1) {
      fprintf(stderr,"Error: l2_write got an error from l1_write\n");
      return(-1);
    }
  }
  return(len);
}

/* l2_read uses l1_read to get bytes. The l2_protocol is that we get
   an initial byte that indicates the length of the chunk (up to 10 bytes of data).
*/


int l2_read(char *chunk, int max) {
  int len=0;
  char x;
  int i;


  if (generror==2) return(-1);

  // first get the chunk length

  if (l1_read(&x)!=1) {
    fprintf(stderr,"Error: l2_read got an error from l1_read\n");
    return(-1);
  }

  len=x;

  if (len<0 || len > max) {
    fprintf(stderr,"Error: l2_read got an invalid chunk len\n");
    return(-1);
  }

  for (i=0;i<len;i++) {
    if (l1_read(chunk+i)!=1) {
      fprintf(stderr,"Error: l2_read got an error from l1_read\n");
      return(-1);
    }
  }

  return(len);
}

//------------------------------------------------------------------
// layer3 prototypes

int l3_write(char *, int);
int l3_read(char *, int);


int main(int argc,char **argv) {

  char buff[4000];
  char inbuff[4000];
  int len;
  int err=0;
  int errcount=0;
  int i;
  int test;

  if (argc == 1) {
    test = 0;
  }
  else {
    test = atoi (argv[1]);
  }

  if (! test || test == 1) {
    // try a small string
    init_l1(); err=0;
    printf("** l3 small string test: ");
    if (l3_write("Hello",6)==-1) {
      printf("failed: write error\n"); err=1;
    } else {
      len = l3_read(buff,1000);
      if (len==-1) {
        printf("failed: read error\n"); err=1;
      } else {
        if (len != 6) {
          printf("failed: read returned wrong length\n"); err=1;
        } else {
          if (strncmp("Hello",buff,6)!=0) {
            printf("failed: message is not right\n"); err=1;
          } else {
            printf("OK\n");
          }
        }
      }
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 3 test case 1 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 3 test case 1 passed.\n");
    }
  }


  if (! test || test == 2) {
    // try a large string
    memset(inbuff,'H',599);
    inbuff[599]=0;
    init_l1(); err=0;
    printf("** l3 long string test: ");
    if (l3_write(inbuff,600)==-1) {
      printf("failed: write error\n"); err=1;
    } else {
      len = l3_read(buff,2000);
      if (len==-1) {
        printf("failed: read error\n"); err=1;
      } else {
        if (len != 600) {
        printf("failed: read returned wrong length\n"); err=1;
        } else {
          if (strncmp(inbuff,buff,600)!=0) {
            printf("failed: message is not right\n"); err=1;
          } else {
            printf("OK\n");
          }
        }
      }
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 3 test case 2 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 3 test case 2 passed.\n");
    }
  }

  if (! test || test == 3) {
    // try different sizes 1-20
    for (i=0;i<100;i++)
    inbuff[i]=i;
    init_l1(); err=0;
    printf("** l3 message length tests: ");
    for (i=1;i<22;i++) {
    if (l3_write(inbuff,i)==-1) {
      printf("failed: write error (%d)\n",i); err=1;
    } else {
      len = l3_read(buff,2000);
      if (len==-1) {
        printf("failed: read error (%d)\n",i); err=1;
      } else {
        if (len != i) {
          printf("Length = %d Returned = %d\n", i, len);
          printf("failed: read returned wrong length (%d)\n",i); err=1;
        } else {
          if (memcmp(inbuff,buff,i)!=0) {
            printf("failed: message is not right (%d)\n",i); err=1;
          }
        }
      }
    }
    if (err) break;
    }
    if (! err) printf("OK\n");
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 3 test case 3 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 3 test case 3 passed.\n");
    }
  }

  if (! test || test == 4) {
    // try a message with all possible byte values
    for (i=0;i<600;i++)
    inbuff[i]=i;
    init_l1(); err=0;
    printf("** l3 message content test: ");
    if (l3_write(inbuff,600)==-1) {
      printf("failed: write error\n"); err=1;
    } else {
      len = l3_read(buff,2000);
      if (len==-1) {
        printf("failed: read error \n"); err=1;
      } else {
        if (len != 600) {
          printf("Length = %d Returned = %d\n", 600, len);
          printf("failed: read returned wrong length \n"); err=1;
        } else {
          if (memcmp(inbuff,buff,600)!=0) {
            printf("failed: message is not right\n"); err=1;
          } else {
            printf("OK\n");
          }
        }
      }
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 3 test case 4 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 3 test case 4 passed.\n");
    }
  }

  if (! test || test == 5) {
    // force read buffer to be too small
    // check for overflow!
    for (i=0;i<100;i++) {
      buff[i]=0;
      inbuff[i]=-1;
    }

    init_l1(); err=0;
    printf("** l3 message too big for buffer test: ");
    if (l3_write(inbuff,100)==-1) {
      printf("failed: write error\n"); err=1;
    } else {
      len = l3_read(buff,20);
      if (len!=-1) {
        printf("failed: l3 read didn't return error\n");
        err=1;
      } else {
        printf("OK\n");
      }
      if (buff[20]!=0) {
        printf("BUFFER OVERFLOW! (bytes modified)\n");
        printf ("\n!! Grading Test Result : Layer 3 test case 5 : Buffer overflow.\n");
      }
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 3 test case 5 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 3 test case 5 passed.\n");
    }
  }

  if (! test || test == 6) {
    // force write error in layer 2
    init_l1(); err=0;
    printf("** l3 forced write error: ");
    generror=1;
    if (l3_write(inbuff,100)!=-1) {
      printf("failed: no error returned\n"); err=1;
    } else {
      printf("OK\n");
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 3 test case 6 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 3 test case 6 passed.\n");
    }
  }

  if (! test || test == 7) {
    // force read error in layer 2
    init_l1(); err=0;
    printf("** l3 forced read error: ");
    generror=2;
    if (l3_write(inbuff,100)==-1) {
      printf("failed: can't get l3_write to work\n"); err=1;
    } else {
      len = l3_read(buff,100);
      if (len!=-1) {
        printf("failed: no error returned\n"); err=1;
      } else {
        printf("OK\n");
      }
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 3 test case 7 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 3 test case 7 passed.\n");
    }
  }

  if (! test) {
    printf("*** Layer 3 score (20 points max): %.1f\n",calc_score(errcount,7,20));
  }

  return(0);
}
