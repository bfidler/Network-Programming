#include <stdio.h>	/* standard C i/o facilities */
#include <unistd.h>	/* Unix System Calls */
#include <sys/types.h>	/* system data type definitions */
#include <sys/socket.h> /* socket specific definitions */
#include <netinet/in.h> /* INET constants and stuff */
#include <arpa/inet.h>	/* IP address conversion stuff */
#include <stdlib.h>
#include <string.h>
#include <netdb.h> /*for gethostbyname*/

/* Server main routine - this is an iterative server

   1. create a socket
   2. bind the socket and print out the port number and ip address
   3. put the socket into passive mode (listen)
   4. wait for connection
        get next connection
        handle the connection
        end connection once client closed
*/


int main(int argc, char**argv) {
  
  int sockfd,newsockfd, portno;
  struct sockaddr_in skaddr;
  struct sockaddr_in cliaddr;
  int addrlen,length;
  int serverOn = 1;
  
  /*getting ip address of server*/
  struct hostent *koala6 = gethostbyname("koala6.cs.clemson.edu");

  char buff[1000];
  char eot = (char)0x04;
  int n;

  /*Checking for arguments*/
  if (argc < 2) {
    printf("Error, no port number provided.\n");
    exit(0);
  }

  /* create a socket
       IP protocol family (PF_INET)
       TCP protocol (SOCK_STREAM)
  */

  if ((sockfd = socket( PF_INET, SOCK_STREAM, 0 )) < 0) {
    perror("Problem creating socket\n");
    exit(1);
  }

  /*establish our address
   address family is AF_INET
   IP address is koala6's
   the port number is assigned by the command line
  */
  portno = atoi(argv[1]);
  skaddr.sin_family = AF_INET;
  memcpy(&skaddr.sin_addr.s_addr, koala6->h_addr_list[0], 
    sizeof(struct in_addr));
    
  if (portno > 65535) {
    printf("Error, port number to big.\n");
    exit(1);
  }
  else {
    skaddr.sin_port = htons(portno);
  }
  /*Checking for error binding*/
  if (bind(sockfd, (struct sockaddr *) &skaddr, sizeof(skaddr))<0) {
    perror("Problem binding\n");
    exit(0);
  }

  
  /*getting the name of the socket*/
  length = sizeof( skaddr );
  if (getsockname(sockfd, (struct sockaddr *) &skaddr, &length)<0) {
    perror("Error getsockname\n");
    exit(1);
  }
  
  /* print out port and IP address of server*/
  printf("The Server socket port number is %d\n",ntohs(skaddr.sin_port));
  printf("The Server IP address is %s\n", inet_ntoa(skaddr.sin_addr));

  /* put the socket into passive mode (waiting for connections) */
  if (listen(sockfd,5) < 0 ) {
    perror("Error calling listen\n");
    exit(1);
  }

  /* now process incoming connections forever ... */
  while (serverOn) {
    printf("Ready for a connection...\n");
    addrlen=sizeof(skaddr);
    if ( (newsockfd = accept( sockfd, (struct sockaddr*) &skaddr, &addrlen)) < 0) {
      perror("Problem with accept call\n");
      exit(1);
    }

    printf("Got a connection - processing...\n");

    /* Determine the address of the new
       server socket */

    length = sizeof( skaddr );
    if (getsockname(newsockfd, (struct sockaddr *) &cliaddr, &length)<0) {
      perror("Error getsockname\n");
      exit(1);
    }

    /* print out the address of the client  */

    printf("The client port number is %d\n",ntohs(cliaddr.sin_port));
    printf("The client IP ADDRESS is %s\n",inet_ntoa(cliaddr.sin_addr));

    /* read a line
       reverse the line
       print the line
       send end of transmission char to client
    */
    while ((n=read(newsockfd,buff,1000))>0) {
      char rev[1000];

      for (int i = 0; i < n; i++)
        rev[i] = buff[n - i - 1];

      write(1,rev,n);
      write(newsockfd, &eot, 1); 
    }
    
    //close sockets and end server
    printf("\n\nDone with connection - closing\n\n");
    close(newsockfd);
    close(sockfd);
    serverOn = 0;
  }
}
