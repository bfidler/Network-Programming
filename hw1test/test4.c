// Netprog 2003 HW1 submission test program
// This code tests layer 4
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
// layer 3 implementation

// l3_write sends a message of arbitrary size by sending l2 chunks
// followed by a zero length l2_chunk

int l3_write(char *msg, int len) {

  int i=0;
  int sendlen;

  if (generror==1) return(-1);

  // checksum bashing. Modify the middle byte by incrementing it's value
  if (generror==5) {
	msg[len/2]++;
  }

  while (i<len) {
    if (len-i>10)
      sendlen=10;
    else
      sendlen = len-i;

    if (l2_write(msg+i,sendlen)!=sendlen) {
      fprintf(stderr,"Error: l3 write got an error or bad count from l2_write \n");
      return(-1);
    }
    i+=sendlen;
  }

  // send the end of message marker
  if (l2_write(msg,0)!=0) {
      fprintf(stderr,"Error: l3 write got an error or bad count from l2_write\n");
      return(-1);
    }

  return(i);
}


/* l3_read uses l2_read to get chunks. The l3_protocol is that we get
   a message and send it as a bunch of l2 chunks. We send a zero byte
   l2 chunk to mark the end of the transmission.
   l4_read now computes the checksum itself to verify that the message
   is the same as the one that was sent
*/


int l3_read(char *msg, int max) {
  int len=0;
  int cnt;

  if (generror==2) return(-1);
  do {
    if ((cnt = l2_read(msg+len,max-len))<0) {
      fprintf(stderr,"Error: l3_read got an error from l2_read\n");
      return(-1);
    }
    if (cnt>10 || len+cnt>max) {
      fprintf(stderr,"Error: l3_read got invalid count (or message too big) from l2_read\n");
      return(-1);
    }
    len+=cnt;
  } while (cnt);
  return(len);
}



//------------------------------------------------------------------
// layer4 prototypes

int l4_write(char *, int);
int l4_read(char *, int);


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
    printf("** l4 small string test: ");
    if (l4_write("Hello",6)==-1) {
  	printf("failed: write error\n"); err=1;
    } else {
  	len = l4_read(buff,1000);
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
      printf ("\n!! Grading Test Result : Layer 4 test case 1 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 4 test case 1 passed.\n");
    }
  }

  if (! test || test == 2) {  
    // try a large string
    memset(inbuff,'H',1000);
    inbuff[1000]=0;
    init_l1(); err=0;
    printf("** l4 long string test: ");
    if (l4_write(inbuff,1001)==-1) {
  	printf("failed: write error\n"); err=1;
    } else {
  	len = l4_read(buff,2000);
  	if (len==-1) {
  	  printf("failed: read error\n"); err=1;
  	} else {
  	  if (len != 1001) {
  		printf("failed: read returned wrong length\n"); err=1;
  	  } else {
  		if (strncmp(inbuff,buff,1001)!=0) {
  		  printf("failed: message is not right\n"); err=1;
  		} else {
  		  printf("OK\n");
  		}
  	  }
  	}
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 4 test case 2 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 4 test case 2 passed.\n");
    }
  }    
  
  if (! test || test == 3) {  
    // force read buffer to be too small
    // check for overflow!
    for (i=0;i<100;i++) {
  	buff[i]=0;
  	inbuff[i]=-1;
    }
  
    init_l1(); err=0;
    printf("** l4 message too big for buffer test: ");
    if (l4_write(inbuff,100)==-1) {
  	printf("failed: write error\n"); err=1;
    } else {
  	len = l4_read(buff,20);
  	if (len!=-1) {
  	  printf("failed: l4 read didn't return error\n");
  	  err=1;
  	} else {
  	  printf("OK\n");
  	}
  	if (buff[20]!=0) {
  	  printf("BUFFER OVERFLOW! (bytes modified)\n");
          printf ("\n!! Grading Test Result : Layer 4 test case 3 : Buffer overflow.\n");
  	}
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 4 test case 3 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 4 test case 3 passed.\n");
    }
  }
  
  if (! test || test == 4) {  
    // force write error in layer 3
    init_l1(); err=0;
    printf("** l4 forced write error: ");
    generror=1;
    if (l4_write(inbuff,100)!=-1) {
  	printf("failed: no error returned\n"); err=1;
    } else {
  	printf("OK\n");
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 4 test case 4 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 4 test case 4 passed.\n");
    }
  }
  
  if (! test || test == 5) {  
    // force read error in layer 3
    init_l1(); err=0;
    printf("** l4 forced read error: ");
    generror=2;
    if (l4_write(inbuff,100)==-1) {
  	printf("failed: can't get l4_write to work\n"); err=1;
    } else {
  	len = l4_read(buff,100);
  	if (len!=-1) {
  	  printf("failed: no error returned\n"); err=1;
  	} else {
  	  printf("OK\n");
  	}
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 4 test case 5 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 4 test case 5 passed.\n");
    }
  }
  
  if (! test || test == 6) {  
    // Finally - the good stuff
    // mess with the checksum!
    init_l1(); err=0;
    for (i=0;i<100;i++) {
  	buff[i]=0;
  	inbuff[i]=i;
    }
  
    printf("** l4 Checksum error: ");
    generror=5;
    if (l4_write(inbuff,100)==-1) {
  	printf("failed: can't get l4_write to work\n"); err=1;
    } else {
  	len = l4_read(buff,100);
  	if (len!=-1) {
  	  printf("failed: no error returned\n"); err=1;
  	} else {
  	  printf("OK\n");
  	}
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 4 test case 6 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 4 test case 6 passed.\n");
    }
  }

  if (! test)
  printf("*** Layer 4 score (20 points max): %.1f\n",calc_score(errcount,6,20)); 
  return(0);

}



      
