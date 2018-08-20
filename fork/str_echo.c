#include "my_unp.h"

char buf[MAXLINE];
int n;

void str_echo(int connfd){
  for(;;){
    if( (n = Read(connfd, buf, MAXLINE)) > 0)
      Write(connfd, buf, n);
    else if(n == 0)
      break;
  }
}
