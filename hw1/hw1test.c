/*
 This code can be used to test your layer 5 implementation
   (which in turn should use your layer 4 implementation, etc.)

 Included is a set of functions that can be used as a layer 1
   implementation, and a main program.

*/

#include <stdio.h>	/* printf, fprintf, etc */
#include <unistd.h>   	/* need this for read(), write() */
#include <stdlib.h>	/* needed for exit()  */
#include <string.h>	/* strlen, etc */


/* ------------------------------------------------------------------
   Sample layer 1 implementation - this can be used to
   provide half-duplex communication by using the shell to
   create a pipe between a sender process and a receiver process.
*/


/* sample l1_read just calls read on stdin */

int l1_read( char *b) {
  return(read(0,b,1));
}

/* sample l1_write just calls write to stdout */

int l1_write(char b) {
  return(write(1,&b,1));
}

//------------------------------------------------------------------
// layer 5 prototypes

int l5_write(char *, int , char *, int );
int l5_read(char *, int *, char *, int *);


/*------------------------------------------------------------------
   Main program. This program can be a sender or a receiver,
    it depends on how many command line arguments are supplied
    when the program is run. 
*/

int main(int argc,char **argv) {

  char namebuf[101];
  char valuebuf[101];
  int namelen;
  int valuelen;

  /* If there are 2 command line arguments then this program will send
       the first (argv[1]) as the "name" and the second as the "value"
       It uses l5_write to send the name/value pair
     If there are not 2 command line arguments, the program assumes it should
       be a reader, so it calls l5_read to get the named value.
  */

  if (argc!=3) {

    /* I'm a reader - read the named value */

    /* IMPORTANT: l5_read requires that namelen and valuelen have values that
          indicate how large the buffers namebuf and valuebuf are. They must
          be initialized before calling l5_read.
    */

    namelen = 100;
    valuelen = 100;

    if (l5_read(namebuf,&namelen,valuebuf,&valuelen)==-1) {
      fprintf(stderr,"Reading error\n");
      exit(1);
    } 

    /* we expect C strings (with null termination), but make sure each is null terminated
       (what if a bad guy sent the message?) before using as a C string
    */

    namebuf[namelen]='\0';
    valuebuf[valuelen]='\0';

    /* print out the name and value received  */

    printf("Name: %s\n",namebuf);
    printf("Value: %s\n",valuebuf);

  } else {

    /* I'm a writer (argv == 3) 
        the first command line argument is the name, the second is the value.
        our application level protocol (above layer 5) has an assumption that neither of
        these should be larger than 100 bytes and that both are C strings (a bunch of
        ASCII chars followed by a terminating null). The 100 byte limit includes the null! 
    */

    if (strlen(argv[1])>=100) {
      fprintf(stderr,"Error - name is too long\n");
      exit(1);
    }

    if (strlen(argv[2])>=100) {
      fprintf(stderr,"Error - value is too long\n");
      exit(1);
    }

    /* lengths are OK - send it
       NOTE: why add one to the lengths? This way the receiver is sent the
       terminating null.
    */

    if (l5_write(argv[1],strlen(argv[1])+1,argv[2],strlen(argv[2])+1)==-1) {
      /* something went wrong when sending */
      fprintf(stderr,"Error sending name and value\n");
      exit(1);
    }

  }
  return(0);
}
