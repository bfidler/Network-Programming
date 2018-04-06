#include <stdio.h>	/* standard C i/o facilities */
#include <unistd.h>	/* Unix System Calls */
#include <sys/types.h>	/* system data type definitions */
#include <sys/socket.h> /* socket specific definitions */
#include <netinet/in.h> /* INET constants and stuff */
#include <arpa/inet.h>	/* IP address conversion stuff */
#include <time.h> /*For appending time to file name*/
#include <stdlib.h> /*standard library*/
#include <string.h> /*Strings*/
#include <pthread.h> /*Posix Threads*/


/*Prototypes*/
char * getTime();
int processConnection(int conn);
static void *doit(void *arg);
/*mutex for locking connfd*/
pthread_mutex_t at_mutex  = PTHREAD_MUTEX_INITIALIZER;

int main (int argc, char**argv) {

  int listenfd, connfd, addrlen, portno;
  struct sockaddr_in skaddr;
  struct sockaddr_in from;
  pthread_t tid;


  /*Checking for port number*/
  if (argc != 2) {
    printf("No port number provided in arguments.\n");
    exit(1);
  }


  /*Creating socket*/
  if ((listenfd = socket( PF_INET, SOCK_STREAM, 0 )) < 0) {
    perror("Problem creating socket\n");
    exit(1);
  }


  /*getting address info*/
  portno = atoi(argv[1]);
  skaddr.sin_family = AF_INET;
  skaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  skaddr.sin_port = htons(portno);


  /*binding skaddr*/
  if (bind(listenfd, (struct sockaddr *) &skaddr, sizeof(skaddr)) < 0) {
    perror("Problem binding\n");
    exit(0);
  }

  //printing port
  printf("The Server port number is %d\n",ntohs(skaddr.sin_port));

  /*Listening for clients*/
  if (listen(listenfd, 5) < 0 ) {
    perror("Error calling listen\n");
    exit(1);
  }

  /*Accepting connections*/
  for ( ; ; ) {
    printf("Ready for a connection...\n");

    addrlen = sizeof(from);
    if ((connfd = accept(listenfd, (struct sockaddr*) &from, &addrlen)) < 0) {
      perror("Problem with accept call\n");
      exit(1);
    }

    /* locking before using connfd in call to pthread_create*/
    pthread_mutex_lock(&at_mutex);
    pthread_create(&tid, NULL, &doit, &connfd);
    pthread_mutex_unlock(&at_mutex);
  }


  return 0;
}


/*This function is used when calling pthread_create*/
static void * doit(void *arg){

  /*locking our arg and converting it to be used as a fd*/
  pthread_mutex_lock(&at_mutex);
  int connfd = * ((int *) arg);
  pthread_mutex_unlock(&at_mutex);

  pthread_detach(pthread_self());

  /*checking for error processing connection*/
  if(processConnection(connfd) == -1) {
    close(connfd);
    pthread_exit((void *) -1);
  }

  close(connfd);
  return (NULL);
}

/*This function gets the current time and returns it as a string*/
char * getTime(){
  time_t myTime = time(NULL);
  char * myTime_str = ctime(&myTime);
  myTime_str[strlen(myTime_str)-1] = *"\0";
  return myTime_str;
}


int processConnection (int conn){

  char buff[9999];
  char filename[256];
  int n;

  /*Reading file name from client*/
  if(read(conn, filename, 256) == -1) {
    printf("Error, receiving data file\n");
    return -1;
  }

  /*creating a unique file based on time*/
  strncat(filename, "-", 255 - sizeof(filename));
  strncat(filename, getTime(), 255 - sizeof(filename));
  strncat(filename, ".txt", 255 - sizeof(filename));
  FILE *fp = fopen(filename, "w");

  if (fp == NULL) {
    printf("Problem creating a file to write\n");
    return -1;
  }

  /*sending ack to client that filename was received*/
  buff[0] = (char)0x04;
  write(conn, buff, 1);

  //sending filename to client
  write (conn, filename, strlen(filename));

  /*Reading from buffer line by line*/
  while ((n = read(conn, buff, 9999)) > 0) {
    char rev[9999];

    /*Reversing each line*/
    for (int i = 0; i < n; i++)
      rev[i] = buff[n - i - 1];

    //Writing to file
    fwrite(rev, 1, n, fp);

    /*Sending ack to client*/
    buff[0] = (char)0x06;
    write(conn, buff, 1);
  }

  fclose(fp);
  return 0;
}
