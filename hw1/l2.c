
#include "l1.h"
#include "l2.h"
#include <stdbool.h>
#include <stdio.h>

int l2_write(char * chunk, int len){
   int ret = 0,
       i=0;

   if(len <= 10 && len >= 0){
      /* attempt to send message */
      ret = l1_write((char) (0xFF & len));

      /* write bytes sequentially as long as there is no error */
      for(; ret != -1 && i < len; ++i)
         ret = l1_write(*(chunk+i));

      /* if there was no error, adjust return value */
      if(ret != -1) {
          ret = len;
	        l1_write((char) 28);
          l1_write((char) 3);
      }

   } else
      /* message too long to be sent */
      ret = -1;

   return ret;
}

int l2_read(char * chunk, int max){
   int ret = 0,
       i=0;
   char len, temp, end;

   /* get chunk size */
   ret = l1_read(&len);

   /* read while unread data remains AND there is no error */
   for(; i < len && ret != -1; ++i){
      ret = l1_read(&temp);

      if(i < max)			/* only write to buffer if there is room */
         chunk[i] = temp;
   }

  l1_read(&temp);
  printf("2: %d\n", len);
	if ((l1_read(&end) != -1 && max >= len)&& temp == (char) 28 && end == (char) 3)
		return len;
	else
		return -1;

   //return (max < len || ret == -1)? -1:len;
}
