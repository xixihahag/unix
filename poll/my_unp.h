#include "my_err.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <poll.h>
#include <errno.h>
#include <sys/stropts.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>

#define SERV_PORT 9877
#define SA struct sockaddr
#define LISTENQ 102400
#define MAXLINE 102400
#define SERVER 00
#define CLIENT 11
#define OPEN_MAX 102400
#define INFTIM -1
#define EPOLL_SIZE 102400

int init(int, char *);
size_t Read(int, void*, size_t);
void Write(int, void*, size_t);
int Accept(int, SA*, socklen_t*);
