#include <stdio.h>	/* standard C i/o facilities */
#include <unistd.h>	/* Unix System Calls */
#include <sys/types.h>	/* system data type definitions */
#include <sys/socket.h> /* socket specific definitions */
#include <netinet/in.h> /* INET constants and stuff */
#include <arpa/inet.h>	/* IP address conversion stuff */

/* Server main routine - this is an iterative server

   1. create a socket
   2. bind the socket and print out the port number assigned
   3. put the socket into passive mode (listen)
   4. do forever
        get next connection
        handle the connection
      enddo
*/


int main() {
  int ld,sd;
  struct sockaddr_in skaddr;
  struct sockaddr_in from;
  int addrlen,length;

  char buff[100];
  int n;

/* create a socket
       IP protocol family (PF_INET)
       TCP protocol (SOCK_STREAM)
*/

  if ((ld = socket( PF_INET, SOCK_STREAM, 0 )) < 0) {
    perror("Problem creating socket\n");
    exit(1);
  }

/* establish our address
   address family is AF_INET
   our IP address is INADDR_ANY (any of our IP addresses)
   the port number is assigned by the kernel
*/

  skaddr.sin_family = AF_INET;
  skaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  skaddr.sin_port = htons(0);

  if (bind(ld, (struct sockaddr *) &skaddr, sizeof(skaddr))<0) {
    perror("Problem binding\n");
    exit(0);
  }

/* find out what port we were assigned and print it out */

  length = sizeof( skaddr );
  if (getsockname(ld, (struct sockaddr *) &skaddr, &length)<0) {
    perror("Error getsockname\n");
    exit(1);
  }
  printf("The Server passive socket port number is %d\n",ntohs(skaddr.sin_port));
  printf("The Server passive socket address is %d\n",ntohs(skaddr.sin_addr.s_addr));

/* put the socket into passive mode (waiting for connections) */

  if (listen(ld,5) < 0 ) {
    perror("Error calling listen\n");
    exit(1);
  }

/* now process incoming connections forever ... */

  while (1) {
    printf("Ready for a connection...\n");
    addrlen=sizeof(skaddr);
    if ( (sd = accept( ld, (struct sockaddr*) &from, &addrlen)) < 0) {
      perror("Problem with accept call\n");
      exit(1);
    }

    printf("Got a connection - processing...\n");

    /* Determine and print out the address of the new
       server socket */

    length = sizeof( skaddr );
    if (getsockname(sd, (struct sockaddr *) &skaddr, &length)<0) {
      perror("Error getsockname\n");
      exit(1);
    }
    printf("The active server port number is %d\n",ntohs(skaddr.sin_port));
    printf("The active server IP ADDRESS is %s\n",inet_ntoa(skaddr.sin_addr));

    /* print out the address of the client  */

    printf("The client port number is %d\n",ntohs(from.sin_port));
    printf("The client IP ADDRESS is %s\n",inet_ntoa(from.sin_addr));

    /* read and send to stdout until the client closes the connection */

    while ((n=read(sd,buff,100))>0) {
      write(1,buff,n);
    }

    printf("Done with connection - closing\n\n\n");
    close(sd);
  }
}
