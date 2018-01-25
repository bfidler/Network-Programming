// Netprog 2003 HW1 submission test program
// This code tests layer 5
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
// layer 4 implementation

// l4_write sends a message with error detection (a checksum)
//  (could do this other ways!)
//
//  we build a buffer that holds the checksum and the data

int l4_write(char *msg, int len) {

  char cksum=0;
  char buf[len+1];
  int i;

  if (generror==1) return(-1);
  // compute the checksum and copy the message to our buffer

  for (i=0;i<len;i++) {
    cksum += msg[i];
    buf[i] = msg[i];
  }
  buf[len]=cksum;

  // now ask layer 3 to send it

  if (l3_write(buf,len+1)!=len+1) {
    fprintf(stderr,"Error: l4_write got an error or short count from l3_write\n");
    return(-1);
  }
  return(len);
}


/* l4_read uses l3_read to get a message that includes the checksum
   l4_read now computes the checksum itself to verify that the message
   is the same as the one that was sent
*/


int l4_read(char *msg, int max) {
  int len;
  char cksum=0;
  char buff[max+1];
  int i;

  if (generror==2) return(-1);
  /* ask l3_read to get an incoming message. We assume the last byte is
     a checksum
  */ 

  if ((len = l3_read(buff,max+1))<=0) {
    fprintf(stderr,"Error: l4_read got an error from l3_read()\n");
    return(-1);
  }

  if (len > max+1) {
    fprintf(stderr,"Error: l4_read got a bad count from l3_read (too big)\n");
    return(-1);
  }

  for (i=0;i<len-1;i++) {
    cksum+=buff[i];
    msg[i]=buff[i];
  }

  if (cksum != buff[len-1]) {
    fprintf(stderr,"Error: l4_read got invalid checksum\n");
    return(-1);
  }
  return(len-1);
}
  




//------------------------------------------------------------------
// layer5 prototypes

int l5_write(char *, int, char *,int);
int l5_read(char *, int *, char *, int *);


int main(int argc,char **argv) {

  char nbuff[4000];
  char vbuff[4000];
  int namelen,vallen;
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
    // try small strings
    init_l1(); err=0;
    printf("** l5 small strings test: ");
    if (l5_write("Hello",6,"There",6)==-1) {
  	printf("failed: write error\n"); err=1;
    } else {
  	namelen=100;
  	vallen=100;
  	len = l5_read(nbuff,&namelen,vbuff,&vallen);
  	if (len==-1) {
  	  printf("failed: read error\n"); err=1;
  	} else {
  	  if ((namelen != 6) || ( vallen!=6)) {
  		printf("failed: read returned wrong length\n"); err=1;
  	  } else {
  		if ((strncmp("Hello",nbuff,6)!=0) || (strncmp(vbuff,"There",6)!=0)) {
  		  printf("failed: name or value is not right\n"); err=1;
  		} else {
  		  printf("OK\n");
  		}
  	  }
  	}
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 5 test case 1 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 5 test case 1 passed.\n");
    }
  }    
  
  if (! test || test == 2) {  
    // try large strings
    memset(nbuff,'n',1000);
    memset(vbuff,'v',1000);
    nbuff[1000]=0;
    vbuff[1000]=0;
    init_l1(); err=0;
    printf("** l5 long string test: ");
    if (l5_write(nbuff,1000,vbuff,1000)==-1) {
  	printf("failed: write error\n"); err=1;
    } else {
  	namelen=2000;
  	vallen=2000;
  	memset(nbuff,0,2000);
  	memset(vbuff,0,2000);
  	len = l5_read(nbuff,&namelen,vbuff,&vallen);
  	if (len==-1) {
  	  printf("failed: read error\n"); err=1;
  	} else {
  	  if ((namelen != 1000) || ( vallen!=1000)) {
  		printf("failed: read returned wrong length\n"); err=1;
  	  } else {
  		for (i=0;i<1000;i++) {
  		  if ((nbuff[i]!='n') || (vbuff[i]!='v')) {
  			printf("failed: name or value is not right\n"); err=1; break;
  		  } 
  		}
  	  }
  	}
    }
    if (!err)  printf("OK\n");
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 5 test case 2 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 5 test case 2 passed.\n");
    }
  }
  
  if (! test || test == 3) {  
    // try all possible byte values in the name,value
    for (i=0;i<1000;i++) {
  	nbuff[i]=vbuff[i]=i; 
    }
    init_l1(); err=0;
    printf("** l5 byte values test: ");
    if (l5_write(nbuff,1000,vbuff,1000)==-1) {
  	printf("failed: write error\n"); err=1;
    } else {
  	namelen=2000;
  	vallen=2000;
  	len = l5_read(nbuff,&namelen,vbuff,&vallen);
  	if (len==-1) {
  	  printf("failed: read error\n"); err=1;
  	} else {
  	  if ((namelen != 1000) || ( vallen!=1000)) {
  		printf("failed: read returned wrong length\n"); err=1;
  	  } else {
  		for (i=0;i<1000;i++) {
  		  if ((nbuff[i]!=(char)i) || (vbuff[i]!=(char)i)) {
  			printf("failed: name or value is not right (%d)\n",i); err=1; break;
  		  } 
  		}
  	  }
  	}
    }
  
    if (!err)  printf("OK\n");
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 5 test case 3 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 5 test case 3 passed.\n");
    }
  }
  
  if (! test || test == 4) {    
    // try to force an overflow
    memset(nbuff,5,1000);
    memset(vbuff,5,1000);
    init_l1(); err=0;
    printf("** l5 overflow test: ");
    if (l5_write(nbuff,1000,vbuff,1000)==-1) {
  	printf("failed: write error\n"); err=1;
    } else {
  	memset(nbuff,0,2000);
  	memset(vbuff,0,2000);
  	namelen=100;
  	vallen=100;
  	len = l5_read(nbuff,&namelen,vbuff,&vallen);
  	if (len!=-1) {
  	  printf("failed: read should return error\n"); err=1;
  	} else {
  	  printf("OK\n");
  	}
  	if ( (nbuff[100]!=0) || (vbuff[100]!=0)) {
  	  printf("BUFFER OVERFLOW byte values modified!\n");
          printf ("\n!! Grading Test Result : Layer 5 test case 4 : Buffer overflow.\n");
  	}
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 5 test case 4 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 5 test case 4 passed.\n");
    }
  }  
  
  if (! test || test == 5) {  
    // force write error in layer 4
    init_l1(); err=0;
    printf("** l5 forced write error: ");
    generror=1;
    if (l5_write(nbuff,100,vbuff,100)!=-1) {
  	printf("failed: no error returned\n"); err=1;
    } else {
  	printf("OK\n");
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 5 test case 5 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 5 test case 5 passed.\n");
    }
  }  
  
  if (! test || test == 6) {    
    // force read error in layer 4
    init_l1(); err=0;
    printf("** l5 forced read error: ");
    generror=2;
    if (l5_write(nbuff,100,vbuff,100)==-1) {
  	printf("failed: can't get l5_write to work\n"); err=1;
    } else {
  	namelen=1000; vallen=1000;
  	len = l5_read(nbuff,&namelen,vbuff,&vallen);
  	if (len!=-1) {
  	  printf("failed: no error returned\n"); err=1;
  	} else {
  	  printf("OK\n");
  	}
    }
    errcount+=err;
    if (err) {
      printf ("\n!! Grading Test Result : Layer 5 test case 6 failed.\n");
    }
    else {
      printf ("\n!! Grading Test Result : Layer 5 test case 6 passed.\n");
    }
  }

  if (! test)
  printf("*** Layer 5 score (20 points max): %.1f\n",calc_score(errcount,6,20)); 
  return(0);

}



      
