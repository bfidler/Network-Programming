#define VERSION_NUMBER 1
%#include <string.h>

/*Operands for functions*/
struct flName {
  string name<>;
};

/*File Server program*/
program FILE_SERVER {
  version FILE_VERSION {
    string getFile (flName) = 1;
  } = VERSION_NUMBER;
} = 2222222;
