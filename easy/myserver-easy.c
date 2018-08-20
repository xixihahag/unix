#include "my_unp.h"

int main(int argc, char **argv) {
  int connfd, n;
  char buf[MAXLINE];
  int listenfd = init(SERVER, NULL);

  for(;;){
    connfd = Accept(listenfd, NULL, NULL);
    for(;;){
      if( (n = Read(connfd, buf, MAXLINE)) > 0)
        Write(connfd, buf ,n);
      else if (n == 0){
        close(connfd);
        break;
      }
    }
  }

  return 0;
}
