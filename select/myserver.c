#include "my_unp.h"

int main(int argc, char const *argv[]) {
  int listenfd = init(SERVER, NULL);
  int connfd, client[FD_SETSIZE], sockfd;
  char buf[MAXLINE];
  fd_set rset, allset;
  int nready, maxfd, i, maxi = -1, n;

  FD_ZERO(&rset);
  FD_ZERO(&allset);
  FD_SET(listenfd, &allset);
  maxfd = listenfd;

  for(int i=0; i<FD_SETSIZE; i++)
    client[i] = -1;

  for(;;){
    rset = allset;
    nready = select(maxfd+1, &rset, NULL, NULL, NULL);

    if(FD_ISSET(listenfd, &rset)){
      connfd = Accept(listenfd, NULL, NULL);
      for(i=0; i<FD_SETSIZE; i++){
        if(client[i] == -1){
          client[i] = connfd;
          break;
        }
      }

      if(i == FD_SETSIZE)
        err_sys("too mant clients");

      FD_SET(connfd, &allset);
      if(connfd > maxfd)
        maxfd = connfd;
      if(maxi < i)
        maxi = i;

      if(--nready <= 0)
        continue;
    }

    for(i=0; i<=maxi; i++){
      if( (sockfd = client[i]) == -1)
        continue;
      if(FD_ISSET(sockfd, &rset)){
        if( (n = Read(sockfd, buf, MAXLINE)) > 0)
          Write(sockfd, buf, n);
        else if(n == 0){
          close(sockfd);
          FD_CLR(sockfd, &allset);
          client[i] = -1;
        }

        if(--nready <= 0)
          break;
      }
    }
  }

  return 0;
}
