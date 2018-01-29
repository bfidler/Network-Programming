// This code tests layer 2
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // read() and write() prototypes are here
#include <string.h>
#include "l1.h"		   // sample layer 1


//------------------------------------------------------------------
// layer2 prototypes

int l2_write(char *, int);
int l2_read(char *, int);


int main(int argc,char **argv) {

  char buff[4000];
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
    // try a simple string first.
    init_l1();  // initialize layer 1
  
    printf("** l2 test - simple string: ");
    if (l2_write("Hello",6)==-1) {
  	printf("failed: l2_write returned error\n"); err=1;
    } else {
  	len = l2_read(buff,10);
  	if (len==-1) {
  	  printf("failed: l2_read returned error\n");	err=1;
  	} else if (len != 6) {
  	  printf("failed: l2_read returned wrong value\n");	err=1;
  	} else if (strncmp("Hello",buff,6)!=0) {
  	  printf("failed: l2_read returned wrong chunk\n");	err=1;
  	} else {
  	  printf("OK\n");
  	}
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 2 test case 1 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 2 test case 1 passed.\n");
    }
  }
  
  
  if (! test || test == 2) {  
    // now try sending some ints
    init_l1(); err=0;
    printf("** l2 test (sending ints): ");
    for (i=0;i<1000;i++) {
      if (l2_write((char *) &i, sizeof(int) )==-1) {
        printf("failed: l2_write error\n");
        err=1;   break;
      }
  
      if ( (len=l2_read(buff,100))==-1) {
        printf("failed: l2_read error\n");
        err=1;   break;
      }
  
      if (len!=sizeof(int)) {
        printf("failed: l2_read returns wrong length\n");
        err=1;break;
      }
  
      if (*((int *)buff) != i) {
        printf("failed: l2_read got wrong int chunk\n");
        err=1;break;
      }
    }
  
    if (! err) printf("OK\n");
    errcount +=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 2 test case 2 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 2 test case 2 passed.\n");
    }
  }
  
  if (! test || test == 3) {  
    // try 0 length message
    init_l1(); err=0;
    printf("** l2 zero length test: ");
    if (l2_write(buff,0)==-1) {
  	printf("failed: l2_write returned error\n");err=1;
    } else {
  	len = l2_read(buff,10);
  	if (len==-1) {
  	  printf("failed: l2_read returned error\n");err=1;
  	} else if (len !=0) {
  	  printf("failed: l2_read returned wrong value\n");err=1;
  	}
    }
    if (!err) printf("OK\n");
    errcount += err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 2 test case 3 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 2 test case 3 passed.\n");
    }
  }

  if (! test || test == 4) {  
    // try some weird buffer sizes
    init_l1(); err=0;
    printf("** l2 write chunk too large test: ");
    if (l2_write(buff,11)!=-1) {
  	printf("failed: didn't return an error\n");
        err = 1;
    } else {
  	printf("OK\n");
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 2 test case 4 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 2 test case 4 passed.\n");
    }
  }

  if (! test || test == 5) {  
    // make the read buffer too small
    init_l1(); err=0;
    printf("** l2 read buffer size test: ");
    if (l2_write(buff,8)==-1) {
  	printf("can't run: l2_write returned error\n");
    } else {
  	len = l2_read(buff,4);
  	if (len!=-1) {
  	  printf("failed: l2_read didn't catch the error\n"); err=1;
  	} else {
  	  printf("OK\n");
  	}
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 2 test case 5 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 2 test case 5 passed.\n");
    }
  }

  if (! test || test == 6) {  
    // force layer 1 write error
    init_l1(); err=0;
    printf("** l2 write forced l1 error test: ");
    generror=1;
    if (l2_write(buff,4)!=-1) {
  	printf("failed: no error reported\n"); err=1;
    } else {
  	printf("OK\n");
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 2 test case 6 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 2 test case 6 passed.\n");
    }
  }

  if (! test || test == 7) {  
    // force layer 1 read error
    init_l1(); err=0;
    printf("** l2 read forced l1 error test: ");
    generror=2;
    if (l2_write(buff,4)==-1) {
  	printf("can't run: l2_write returned error\n");
    } else {
  	if (l2_read(buff,4)!=-1) {
  	  printf("failed: l2_read didn't return error\n"); err=1;
  	} else {
  	  printf("OK\n");
  	}
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 2 test case 7 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 2 test case 7 passed.\n");
    }
  }

  if (! test)
  printf("*** Layer 2 score (20 points max): %.1f\n",calc_score(errcount,7,20)); 
  return(0);
}



      
