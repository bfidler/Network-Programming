#define VERSION_NUMBER 1
%#include <string.h>

/*Operands for functions*/

struct regName {
  string name<>;
  string hostname<>;
};

struct remName {
  string name<>;
};

struct getName {
  string name<>;
};

/*Name Server program*/
program NAME_SERVER {
  version NAME_VERSION {
    int registerServer (regName) = 1;
    int removeServer(remName) = 2;
    string getServerHostName(getName) = 3;
  } = VERSION_NUMBER;
} = 33333333;
