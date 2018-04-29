/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

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
        result = (char *) malloc(256*sizeof(char));
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
          int len = strlen(namebuff);

          /*Removing new line char*/
          if (len > 0 && namebuff[len - 1] == '\n') {
            namebuff[--len] = '\0';
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
        result[0] = '\0';
        fclose(fp);
        return &result;
}
