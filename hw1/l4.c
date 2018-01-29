#include "l3.h"
#include "l4.h"
#include <stdbool.h>
#include <stdio.h>

int l4_write(char *msg, int len)
{
  int i;
  int sum = 0;
  int wrapSum, checkSum, sent;
  char sendMsg[len + 1];

  //calculating sum of the message and giving it to the string
  for (i = 0; i < len; i++) {
    sum += (int) msg[i];
    sendMsg[i] = msg[i];
  }

  //calculating checkSum then putting it in a string
  wrapSum = sum % 256;
  checkSum = 256 - wrapSum;
  sendMsg[len] = (char) checkSum;

  //sending the message
  sent = l3_write(sendMsg, len + 1);

  //returns either - 1 for error or the number of bytes sent
  if (sent == -1)
    return -1;
  else
    return sent - 1;
}

int l4_read(char *msg, int max)
{
  int i, read;
  char readMsg[max + 1];
  int sum = 0;

  //reading the message and getting the length of the message read
  read = l3_read(readMsg, max + 1);

  //checking for errors reading from layer3 and message size too big
  if(read == -1 || read > max + 1)
    return -1;

  //calculating sum of the message and transferring it to msg pointer
  for (i = 0; i < read - 1; i++) {
    sum += (int) readMsg[i];
    msg[i] = readMsg[i];
  }

  //adding the checkSum to the sum
  sum += readMsg[read - 1];

  //checking for transmission errors by modulus 256
  //should give us 0 if the message was sent correctly
  //we then return the length read
  if(sum % 256 == 0)
    return read - 1;
  else
    return -1;
}
