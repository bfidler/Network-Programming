/*

Author: bfidler

Filename: l5.h

Description: This file provides the prototypes for Layer 5.

*/

int l5_write(char *name, int namelen, char *value, int valuelen);
int l5_read(char *name, int *namelenptr, char *value, int *valuelenptr);
