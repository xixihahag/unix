#include "my_unp.h"

void *copyto(void *);

static int sockfd;
static FILE *fp;

void str_cli(FILE *fp_arg, int sockfd_arg){
    sockfd = sockfd_arg;
    fp = fp_arg;
    pthread_t pid;
    int n;
    char buf[MAXLINE];

    pthread_create(&pid, NULL, copyto, NULL);

    while( (n = Read(sockfd, buf, MAXLINE)) > 0)
      Write(fileno(fp), buf, n);
}

void *copyto(void *arg){
  char sendline[MAXLINE];
  int n;

  for(;;){
    if( (n = Read(fileno(fp), sendline, MAXLINE)) > 0)
      Write(sockfd, sendline, n);
    if(n == 0)
      break;
  }

  shutdown(sockfd, SHUT_WR);
}
