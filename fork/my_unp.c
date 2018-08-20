#include "my_unp.h"

int init(int key, char *ipaddress){
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in servaddr;
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);

  if(key == SERVER){
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listenfd, (SA*)&servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);
  }
  else{
    inet_pton(AF_INET, ipaddress, &servaddr.sin_addr);
    connect(listenfd, (SA*)&servaddr, sizeof(servaddr));
  }

  return listenfd;
}

//read(listenfd, &recv, MAXLINE)
size_t Read(int fd, void *ptr, size_t nbytes){
  size_t n;
  if( (n = read(fd, ptr, nbytes)) < 0)
    err_sys("read error\n");
  return n;
}

//write(listenfd, buf, n)
void Write(int fd, void *ptr, size_t nbytes){
  if(write(fd, ptr, nbytes) != nbytes)
    err_sys("write error\n");
}


int Accept(int listenfd, SA *cliaddr, socklen_t *addrlen){
  int connfd;
  if( (connfd = accept(listenfd, cliaddr, addrlen)) < 0)
    err_sys("accept error\n");
  return connfd;
}
