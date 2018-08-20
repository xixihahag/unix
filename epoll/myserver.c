/**
 * [main description]
 * @param  argc [description]
 * @param  argv [description]
 * @return      [description]
 * 问题，需不需要判断对面是否可读
 * 需要的话要怎么读和写传数据，不会错乱么
 *
 */
#include "my_unp.h"

int main(int argc, char const *argv[]) {
  int listenfd = init(SERVER, NULL);
  struct epoll_event ev, events[EPOLL_SIZE];
  int epfd, connfd;
  int nready, i, n;
  char buf[MAXLINE];

  epfd = epoll_create(EPOLL_SIZE);
  ev.data.fd = listenfd;
  ev.events = EPOLLIN;
  epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

  for(;;){
    nready = epoll_wait(epfd, events, EPOLL_SIZE, -1);

    for(i=0; i<nready; i++){
      if(events[i].data.fd == listenfd){
        connfd = Accept(listenfd, NULL, NULL);
        ev.data.fd = connfd;
        ev.events = EPOLLIN;
        epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
      }
      else if(events[i].events & EPOLLIN){
        connfd = events[i].data.fd;
        if( (n = Read(connfd, buf, MAXLINE)) > 0){
            ev.data.fd = connfd;
            ev.events = EPOLLOUT;
            epoll_ctl(epfd, EPOLL_CTL_MOD, connfd, &ev);
        }
        else if(n == 0){
          epoll_ctl(epfd, EPOLL_CTL_DEL, connfd, NULL);
        }
      }
      else if(events[i].events & EPOLLOUT){
        connfd = events[i].data.fd;
        Write(connfd, buf, n);
        ev.data.fd = connfd;
        ev.events = EPOLLIN;
        epoll_ctl(epfd, EPOLL_CTL_MOD, connfd, &ev);
      }
    }
  }

  return 0;
}
