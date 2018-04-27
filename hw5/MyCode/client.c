/* RPC client for simple addition example */

#include <stdio.h>


/* Wrapper function takes care of calling the RPC procedure */

int main( int argc, char *argv[]) {

  if (argc!=4) {
    fprintf(stderr,"Usage: %s Host name FileServer FileName\n",argv[0]);
    exit(0);
  }

}
