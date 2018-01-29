#include "l2.h"
#include "l3.h"
#include <stdbool.h>

int l3_write(char *msg, int len)
{
   int i;
   int j;
   int sent = 0;
   int temp;
   char chunkBuf[10];
   int chunkCount = len / 10;
   if (len % 10) chunkCount++;

   for (i = 0; (i < chunkCount) && (temp != -1); i++) {
      for (j = 0; (j < 10) && ((j + sent) < len); j++)
 	      chunkBuf[j] = *(msg + (j + sent));

      temp = l2_write(chunkBuf, j);
      sent += temp;
   }

   if (temp == -1) return(-1);

   chunkBuf[0] = (char) 29;
   l2_write(chunkBuf, 1);
   chunkBuf[0]= (char) 3;
   l2_write(chunkBuf, 1);

   return(sent);
}

int l3_read(char *msg, int max)
{
   int i;
   int read = 0;
   int temp;
   char chunkBuf[10];

   while (read < max)
   {
      temp = l2_read(chunkBuf, 10);
      if (temp == -1) return (-1);
      for (i = 0; (i < temp) && ((read + i) < max); i++)
 	      *(msg + (read + i)) = chunkBuf[i];

      read += i;
      if (i < temp) return (-1);
      if (*(msg + read - 1) == (char) 3 && *(msg + read - 2) == (char) 29){
	       return (read - 2); printf("3: %d\n", read - 2); }
   }

   if (read == max)
   {
      if(l2_read(chunkBuf, 1) == -1) return (-1);
      if(chunkBuf[0] == 0x00) return (read - 2);
      else return (-1);
   }

   return (read - 2);
}
