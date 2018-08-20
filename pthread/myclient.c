#include "my_unp.h"
#include "str_cli.h"

int main(int argc, char **argv) {
  int sockfd = init(CLIENT, argv[1]);
  str_cli(stdin, sockfd);
  return 0;
}
