#include "l4.h"
#include "l5.h"

int l5_write(char *name, int namelen, char *value, int valuelen) {

  int sentName, sentVal;

  sentName = l4_write(name, namelen);
  sentVal = l4_write(value, valuelen);

  if(sentName == namelen && sentVal == valuelen)
    return 1;
  else
    return -1;
}


int l5_read(char *name, int *namelenptr, char *value, int *valuelenptr) {

  int readName, readVal;

  readName = l4_read(name, *namelenptr);
  readVal = l4_read(value, *valuelenptr);

  if(readName == -1 || readVal == -1)
    return -1;

  if(readName > *namelenptr || readVal > *valuelenptr)
    return -1;

  *namelenptr = readName;
  *valuelenptr = readVal;

  return readName + readVal;
}

//fix layer 4
