/*

Author: bfidler

Filename: l3.c

Description: This file provides the implementation for Layer 3.

*/

#include "l2.h"
#include "l3.h"
#include <stdbool.h>
#include <stdio.h>

int l3_write(char *msg, int len)
{
   int i;
   int j;
   int sent = 0;
   int temp;
   char chunkBuf[10];
   int chunkCount = len / 10;
   if (len % 10) chunkCount++;

   //sending chunks
   for (i = 0; (i < chunkCount) && (temp != -1); i++) {
      for (j = 0; (j < 10) && ((j + sent) < len); j++)
 	      chunkBuf[j] = *(msg + (j + sent));

      temp = l2_write(chunkBuf, j);
      sent += temp;
   }

   if (temp == -1) return(-1);

   //writing the header at the end of the msg
   chunkBuf[0] = (char) 29;
   chunkBuf[1]= (char) 3;
   l2_write(chunkBuf, 2);

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
      //reading a chunk
      temp = l2_read(chunkBuf, 10);
      if (temp == -1) return (-1);

      //checking for header to signal end of transmission
      if (temp == 2 && chunkBuf[0] == (char) 29 && chunkBuf[1] == (char) 3)
	      return (read);

      //moving chars from buffer to msg
      for (i = 0; (i < temp) && ((read + i) < max); i++)
 	      *(msg + (read + i)) = chunkBuf[i];

      read += i;
      if (i < temp) return (-1);
   }

  if(read == max) {
    l2_read(chunkBuf, 2);
    //looking for null char at end of msg or undread header
    //to signal end of transmission
    if((msg[read - 1] = (char) 0) ||
      (chunkBuf[0] == (char) 29 && chunkBuf[1] == (char) 3)) {
      return read;
    }
    else
      return -1;
    }
    else {
      return -1;
    }
}
