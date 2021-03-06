#include <stdio.h>	/* standard C i/o facilities */
#include <stdlib.h>	/* needed for atoi() */
#include <unistd.h>	/* Unix System Calls */
#include <sys/types.h>	/* system data type definitions */
#include <sys/socket.h> /* socket specific definitions */
#include <netinet/in.h> /* INET constants and stuff */
#include <arpa/inet.h>	/* IP address conversion stuff */

/* define the message we will send */

char *message = "Networking Rules !!!\n";


/* client program:

   The following must passed in on the command line:
   
      name of the server (argv[1])
      port number of the server (argv[2])
*/

int main( int argc, char **argv ) {

  int sk;
  struct sockaddr_in skaddr;

  /* first - check to make sure there are 2 command line parameters
     (argc=3 since the program name is argv[0]) 
  */

  if (argc!=3) {
    printf("Usage: client <server name> <server port>\n");
    exit(0);
  }

  /* create a socket 
     IP protocol family (PF_INET) 
     TCP protocol (SOCK_STREAM)
  */
  
  if ((sk = socket( PF_INET, SOCK_STREAM, 0 )) < 0) {
    printf("Problem creating socket\n");
    exit(1);
  }

  /* fill in an address structure that will be used to specify
     the address of the server we want to connect to

     address family is IP  (AF_INET)

     server IP address is found by calling gethostbyname with the
     name of the server (entered on the command line)

     server port number is argv[2] (entered on the command line)
  */

  skaddr.sin_family = AF_INET;

#ifndef SUN
  if (inet_aton(argv[1],&skaddr.sin_addr)==0) {
    printf("Invalid IP address: %s\n",argv[1]);
    exit(1);
  }
#else
  /*inet_aton is missing on Solaris - you need to use inet_addr! */
  /* inet_addr is not as nice, the return value is -1 if it fails
     (so we need to assume that is not the right address !)
  */

  skaddr.sin_addr.s_addr = inet_addr(argv[1]);
  if (skaddr.sin_addr.s_addr ==-1) {
    printf("Invalid IP address: %s\n",argv[1]);
    exit(1);
  }
#endif
 
  skaddr.sin_port = htons(atoi(argv[2]));
  

  /* attempt to establish a connection with the server */


  if (connect(sk,(struct sockaddr *) &skaddr,sizeof(skaddr)) < 0 ) {
    printf("Problem connecting socket\n");
    exit(1);
  }

  /* Send a string and finish*/

  write(sk,message,strlen(message));

  close(sk);
  return(0);
}













