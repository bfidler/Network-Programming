#define VERSION_NUMBER 1
%#include <string.h>

/*Operands for functions*/

struct regName {
  char name[];
  char hostname[];
};

struct remName {
  char name[];
};

struct getName {
  char name[];
};

/*Name Server program*/
program NAME_SERVER {
  version NAME_VERSION {
    int registerServer (regName) = 1;
    int removeServer(remName) = 2;
    string getServerHostName(getName) = 3;
  } = VERSION_NUMBER;
} = 33333333;
