
#include <stdio.h>
#include "NameServer.h"
#include "FileServer.h"


int main( int argc, char *argv[]) {

  /*Checking for proper arguments*/
  if (argc!=4) {
    fprintf(stderr,"Usage: %s [Host Name] [Name of File Server] [File Name]\n",argv[0]);
    exit(0);
  }

  /*RPC Client first acts as a client to name server*/
  CLIENT *clntNS;
  getName gArgs;

  /*Creating NameServer Client with hostname of NameServer*/
	clntNS = clnt_create(argv[1], NAME_SERVER, NAME_VERSION, "udp");

  /* Make sure the create worked */
  if (clntNS == (CLIENT *) NULL) {
    clnt_pcreateerror(argv[1]);
    exit(1);
  }

  /*Name of FileServer*/
  gArgs.name = argv[2];

  /*Getting host name of the FileServer*/
  char *fileHost = *getserverhostname_1(&gArgs, clntNS);

  /*Checking to make sure file host was found*/
	if(fileHost[0] == '\0'){
		printf("%s\n", "Failed to get host name of FileServer from NameServer");
		exit(1);
	}

  /*Now RPC Client will run as a client to file server*/
  CLIENT *clntFS;
  flName fArgs;

  /*Creating FileServer Client with hostname retrieved from NameServer*/
  clntFS = clnt_create(fileHost, FILE_SERVER, FILE_VERSION, "udp");
  
  if(clntFS == (CLIENT *)NULL) {
    clnt_pcreateerror(fileHost);
    exit(1);
  }

  /*Name of File*/
  fArgs.name = argv[3];

  /*Retrieving file from file server*/
  char *fileString = *getfile_1(&fArgs, clntFS);

  /*Making sure file was retrieved*/
  if(fileString == '\0') {
    printf("%s\n", "Failed to retrieve the file from FileServer");
    exit(1);
  }

  /*Printing file*/
  printf("Retrieved file %s:\n", argv[3]);
  printf("\n%s\n", fileString);
}
