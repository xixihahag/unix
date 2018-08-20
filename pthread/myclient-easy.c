#include "my_unp.h"

int main(int argc, char **argv) {
  int listenfd;
  int n;
  char buf[MAXLINE], recv[MAXLINE];
  listenfd = init(CLIENT, argv[1]);

  for(;;){
    if( (n = Read(fileno(stdin), buf, MAXLINE)) > 0)
        Write(listenfd, buf, n);
    else if(n == 0)
      break;

    if( (n = Read(listenfd, recv, MAXLINE)) > 0)
      Write(fileno(stdout), recv, n);
  }

  return 0;
}
