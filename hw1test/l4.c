#include "l3.h"
#include "l4.h"
#include <stdbool.h>
#include <stdio.h>

int l4_write(char *msg, int len)
{
  int i;
  int sum = 0;
  int wrapSum, checkSum;
  int sent;

  //calculating sum of the message
  for (i = 0; i < len; i++)
    sum += (int) msg[i];

  //calculating checkSum then putting it in a string
  wrapSum = sum % 256;
  checkSum = 256 - wrapSum;
  char checkStr[1] = {checkSum};

  //sending the message
  sent = l3_write(msg, len);

  //checking for errors sending the message and appending the checkSum
  if(sent == -1 || l3_write(checkStr, 1) == -1)
    return -1;
  //returning the length of the message sent
  else
    return sent;
}

int l4_read(char *msg, int max)
{
  int i, read;
  char temp[1];
  int sum = 0;

  //reading the message and getting the length of the message read
  read = l3_read(msg, max);

  //checking for errors reading from layer3 and message size too big
  if(read == -1 || read > max)
    return -1;

  //calculating sum of the message
  for (i = 0; i < read; i++)
    sum += (int) msg[i];

  //adding the checkSum to the sum and checking for its transmission errors
  if(l3_read(temp, 1) == 1)
    sum += temp[0];
  else
    return -1;

  //checking for transmission errors by modulus 256
  //should give us 0 if the message was sent correctly
  //we then return the length read
  if(sum % 256 == 0)
    return read;
  else
    return -1;
}
