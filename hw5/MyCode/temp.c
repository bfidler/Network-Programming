#include "NameServer.h"
#include <string.h>


int *
registerserver_1_svc(regName *argp, struct svc_req *rqstp)
{
        static int result;

        /*Opening configuration file*/
        FILE *fp = fopen("config.txt", "a");

        /*Checking that file was openeded*/
        if (fp == NULL){
           printf("Trouble opening config file.\n");
           result = -1;
           return &result;
        }

        /*Writing name and host on separate lines*/
        fprintf(fp, "%s\n", argp->name);
        fprintf(stdout, "%s\n", argp->name);
        fprintf(fp, "%s\n", argp->hostname);

        fclose (fp);

        /*Success*/
        result = 1;
        return &result;
}

int *
removeserver_1_svc(remName *argp, struct svc_req *rqstp)
{
        static int  result;

        /*
         * insert server code here
         */
         result = 1;

        return &result;
}

char **
getserverhostname_1_svc(getName *argp, struct svc_req *rqstp)
{
        static char * result;

        /*Opening configuration file*/
        FILE *fp = fopen("config.txt", "r");

        /*Checking that file was openeded*/
        if (fp == NULL){
           printf("Trouble opening config file.\n");
           return &result;
        }

        char namebuff[256];

        /*searching for host using the name*/
        while(fgets(namebuff, 256, fp)){
          /*Removing new line char*/
          if (strlen(namebuff) > 0 && namebuff[strlen(namebuff) - 1] == '\n') {
            namebuff[strlen(namebuff)] = '\0';
          }

          /*If the name is found, host is stored and returned in result*/
          if(strcmp(argp->name,namebuff) == 0) {
            fgets(result, 256, fp);
            fclose(fp);
            return &result;
          }
          else {
            fgets(namebuff, 256, fp);
          }
        }

        /*Host not found if function makes it here*/
        printf("Could not locate host name.\n");

        fclose(fp);
        return &result;
}




int
main (int argc, char **argv)
{
	/*Running file server as a client of NameServer first*/
	CLIENT *clnt;
	char name[256] = "FileServer";
	regName ops;

	if (argc!= 2) {
    fprintf(stderr,"Usage: %s Host Name\n",argv[0]);
    exit(0);
  }

	/*Creating Client*/
	clnt = clnt_create(argv[1], NAME_SERVER, NAME_VERSION, "udp");

	/* Make sure the create worked */
  if (clnt == (CLIENT *) NULL) {
    clnt_pcreateerror(argv[1]);
    exit(1);
  }

	ops.name = name;
	ops.hostname = argv[1];

	/*Register host to name server*/
	if(*registerserver_1(&ops, clnt) == -1){
		printf("%s\n", "Failed to register host name with NameServer");
		exit(1);
	}









  #include "FileServer.h"
  #include <string.h>

  char **
  getfile_1_svc(flName *argp, struct svc_req *rqstp)
  {
  	static char * result;

  	FILE *fp = fopen(argp->name, "r");
  	char buff[1000];

  	/*Checking that file was openeded*/
  	if (fp == NULL){
  		 printf("Could not open file.\n");
  		 return &result;
  	}


  	/*Reading file into the result string*/
  	while(fgets(buff, 1000, fp)){
  		strcat(result, buff);
  	}

  	return &result;
  }
