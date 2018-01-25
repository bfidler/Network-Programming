//
// This code can be used to test your layer 3 implementation
//   (which in turn should use your layer 2 implementation, etc.)
//
//
// To use this code, build l3test and run like this:
//    ./l3test foo | ./l3test
//
//   the first l3test has a command line arg (foo), so this will become the message.
//   the second doesn't have any, so it will become a reader.

// Included is a set of functions that can be used as a layer 1
//   implementation, and a main program.
//

#include <stdio.h>	/* printf, fprintf, etc */
#include <unistd.h>   	/* need this for read(), write() */
#include <stdlib.h>	/* needed for exit()  */
#include <string.h>	/* strlen, etc */


//------------------------------------------------------------------
// Sample layer 1 implementation - this can be used to
// provide half-duplex communication by using the shell to
// create a pipe between a sender process and a receiver process.

// sample l1_read just calls read on stdin
int l1_read( char *b) {
  return(read(0,b,1));
}

// sample l1_write just calls write to stdout
int l1_write(char b) {
  return(write(1,&b,1));
}

//------------------------------------------------------------------
// layer 3 prototypes

int l3_write(char *, int);
int l3_read(char *, int);


//------------------------------------------------------------------
// Main program. This program can be a sender or a receiver,
//  it depends on how many command line arguments are supplied
//  when the program is run. 

int main(int argc,char **argv) {

  char buff[5000];
  int len;
  // If there are any command line arguments then this program will 
  // send the first one found using l3_write(), otherwise it's a reader 

  if (argc<=1) {
    // Be a reader - just call l3_read
    if ((len=l3_read(buff,5000))<0) {
      fprintf(stderr,"Error returned from l3_read\n");
    } else {
      printf("Received a message of length %d\n",len);
      printf("Here is the message: <");
      fflush(stdout);
      write(1,buff,len);
      printf(">\n");
    }
  } else {

    // I'm a writer, send the first command line arg 
    // NOTE: we are not sending the terminating null!
    //   (this allows testing of 0 length messages)

    if (l3_write(argv[1],strlen(argv[1]))<0) {
      // something went wrong when sending
      fprintf(stderr,"Error returned from l3_write\n");
    }
  }
  return(0);
}
