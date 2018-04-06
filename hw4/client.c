#include <stdio.h>	/* standard C i/o facilities */
#include <stdlib.h>	/* needed for atoi() */
#include <unistd.h>	/* Unix System Calls */
#include <sys/types.h>	/* system data type definitions */
#include <sys/socket.h> /* socket specific definitions */
#include <netinet/in.h> /* INET constants and stuff */
#include <arpa/inet.h>	/* IP address conversion stuff */
#include <string.h> /* String stuff */

int main( int argc, char **argv ) {

  FILE *data;
  int skt;
  struct sockaddr_in skaddr;
  char buff[9999];


  //checking for correct number of arguments
  if (argc!= 4) {
    printf("Usage: client <server name> <server port> <filename>\n");
    exit(0);
  }


  /*Attempting to open data file*/
  if((data = fopen(argv[3], "r+")) == NULL) {
    printf("Could not open data file.\n");
    exit(1);
  }

  /* Creating socket and checking for errors*/
  if ((skt = socket( PF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Problem creating socket\n");
    exit(1);
  }

  //address family is IP  (AF_INET)
  skaddr.sin_family = AF_INET;


  /*Setting IP address of skaddr based on command line argument of IP address*/
  #ifndef SUN
    if (inet_aton(argv[1],&skaddr.sin_addr)==0) {
      printf("Invalid IP address: %s\n",argv[1]);
      exit(1);
    }
  #else
    /*inet_aton is missing on Solaris - you need to use inet_addr! */
    /*inet_addr is not as nice, the return value is -1 if it fails
      (so we need to assume that is not the right address !)
    */
    skaddr.sin_addr.s_addr = inet_addr(argv[1]);
    if (skaddr.sin_addr.s_addr ==-1) {
      printf("Invalid IP address: %s\n",argv[1]);
      exit(1);
    }
  #endif

  //setting port of skaddr based on command line
  skaddr.sin_port = htons(atoi(argv[2]));


  /* attempt to establish a connection with the server */
  if (connect(skt,(struct sockaddr *) &skaddr,sizeof(skaddr)) < 0 ) {
    printf("Problem connecting skt\n");
    exit(1);
  }

  //sending file name to server
  write(skt, argv[3], strlen(argv[3]));

  /*waiting for ACK from Server*/
  read(skt, &buff, 1);
  if(buff[0] != (char)0x04){
    printf("Error, ACK not received.\n");
    close(skt);
    exit(1);
  }

  /*reading file name from server (different from original)*/
  read(skt, &buff, 9999);
  if (buff == argv[3]) {
    printf("Error, server returned same file name as client.\n");
    close(skt);
    exit(1);
  }


  /*  Send a line
      Wait until transmission character is received
      repeat
  */
  while(fgets(buff, 9999, data)) {
    write(skt, buff ,strlen(buff));
    read(skt, &buff, 1);
    /*Checking for transmission error*/
    if (buff[0] != (char)0x06) {
      printf("Transmission error\n");
      exit(1);
      close(skt);
    }
  }


  /*Sending EoT to server*/
  buff[0] = (char) 0x04;
  write(skt, buff, strlen(buff));


  /*Closing skt and data file*/
  fclose(data);
  close(skt);
  return 0;
}
