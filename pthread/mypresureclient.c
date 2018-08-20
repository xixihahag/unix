#include "my_unp.h"

// char *getline(int);

int main(int argc, char **argv) {
  int listenfd;
  struct sockaddr_in servaddr;
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

  struct epoll_event ev, events[EPOLL_SIZE];
  int epfd, connfd;
  int count = 10000;
  int nready, i, n, recvnum = 0;
  char buf[MAXLINE], sendline[MAXLINE] = "1234567\0";
  struct timeval tpstart, tpend;
  float timeuse;
  int sockfd[EPOLL_SIZE];

  epfd = epoll_create(EPOLL_SIZE);

  gettimeofday(&tpstart, NULL);

  for(i=0; i<count; i++){
      sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
      connect(sockfd[i], (SA*)&servaddr, sizeof(servaddr));
  }

  gettimeofday(&tpend, NULL);
  timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec;
  timeuse /= 1000000;
  printf("conn time %lf s\n", timeuse);

  for(i=0; i<count; i++){
    ev.data.fd = sockfd[i];
    ev.events = EPOLLOUT;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd[i], &ev);
  }

  gettimeofday(&tpstart, NULL);

  for(;;){
    if(recvnum >= count)
      break;
    nready = epoll_wait(epfd, events, EPOLL_SIZE, -1);
    for(i=0; i<nready; i++){
      connfd = events[i].data.fd;
      if(events[i].events & EPOLLOUT){
        //write to SERVER
        // sendline = getline(1);
        Write(connfd, sendline, strlen(sendline));
        ev.data.fd = connfd;
        ev.events = EPOLLIN;
        epoll_ctl(epfd, EPOLL_CTL_MOD, connfd, &ev);
      }
      else if(events[i].events & EPOLLIN){
        //read from server
        if( (n = Read(connfd, buf, MAXLINE)) > 0){
          recvnum++;
          // Write(fileno(stdout), buf, n);
          epoll_ctl(epfd, EPOLL_CTL_DEL, connfd, NULL);
        }
      }
    }
  }

  gettimeofday(&tpend, NULL);
  timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec;
  timeuse /= 1000000;
  printf("echo time %lf s\n", timeuse);

  for(int i=0; i<count; i++)
    shutdown(sockfd[i], SHUT_RDWR);
    // close(sockfd[i]);
    // shutdown(sockfd[i], SHUT_WR);

  close(epfd);
  return 0;
}

// char *getline(int nbytes){
//   //暂时先不管，一律定位8位
//   return "1234567\0";
// }
