#include "l4.h"
#include "l5.h"
#include <stdio.h>

int l5_write(char *name, int namelen, char *value, int valuelen) {

  int sentName, sentVal;

  //sending name then value
  sentName = l4_write(name, namelen);
  sentVal = l4_write(value, valuelen);

  //checking to make sure the correct amount of bytes were sent
  if(sentName == namelen && sentVal == valuelen)
    return 1;
  else
    return -1;
}


int l5_read(char *name, int *namelenptr, char *value, int *valuelenptr) {

  int readName, readVal;

  //reading name of value, using the pointers a maxes of the buffer
  readName = l4_read(name, *namelenptr);
  readVal = l4_read(value, *valuelenptr);

  //making sure name of value were read properly
  if(readName == -1 || readVal == -1)
    return -1;
  printf("5: %d %d\n",readName, readVal);
  //checking to see if name or value overflow buffer
  if(readName > *namelenptr || readVal > *valuelenptr)
    return -1;

  //setting pointers to the number of bytes sent
  *namelenptr = readName;
  *valuelenptr = readVal;

  //returning total bytes sent
  return readName + readVal;
}

//fix layer 4
