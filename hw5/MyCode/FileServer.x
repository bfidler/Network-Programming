#define VERSION_NUMBER 1
%#include <string.h>

/*Operands for functions*/
struct flName {
  char* name;
};

/*File Server program*/
program FILE_SERVER {
  version NAME_VERSION {
    string getFile (flName) = 1;
  } = VERSION_NUMBER;
} = 2222222;
