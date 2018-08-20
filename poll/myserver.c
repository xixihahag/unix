#include "my_unp.h"

int main(int argc, char const *argv[]) {
  int listenfd = init(SERVER, NULL);
  char buf[MAXLINE];
  struct pollfd client[OPEN_MAX];
  int nready, maxi = 0;
  int connfd, i, sockfd, n;

  client[0].fd = listenfd;
  client[0].events = POLLRDNORM;
  for(i=1; i<OPEN_MAX; i++)
    client[i].fd = -1;

  for(;;){
    nready = poll(client, maxi+1, INFTIM);

    if(client[0].revents & POLLRDNORM){
      connfd = Accept(listenfd, NULL, NULL);

      for(i=1; i<OPEN_MAX; i++)
        if(client[i].fd == -1){
          client[i].fd = connfd;
          client[i].events = POLLRDNORM;
          break;
        }

      if(i == OPEN_MAX)
        err_sys("too many clients\n");

      if(i > maxi)
        maxi = i;

      if(--nready <= 0)
        continue;
    }

    for(i=1; i<=maxi; i++){
      if( (sockfd = client[i].fd) == -1)
        continue;
      if( (n = read(sockfd, buf, MAXLINE)) > 0)
        Write(sockfd, buf, n);
      else if(n == 0){
        close(sockfd);
        client[i].fd = -1;
      }
      else if(n < 0){
        if(errno == ECONNRESET){
          close(sockfd);
          client[i].fd = -1;
        }
        else
          err_sys("read error\n");
      }

      if(--nready <= 0)
        continue;
    }
  }

  return 0;
}
