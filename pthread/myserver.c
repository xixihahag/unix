#include "my_unp.h"
#include "str_echo.h"

static void *doit(void *);

int main(int argc, char const *argv[]) {
  int listenfd = init(SERVER, NULL);
  pthread_t tid;
  int *connfd;

  for(;;){
    connfd = malloc(sizeof(int));
    *connfd = Accept(listenfd, NULL, NULL);

    // int flag = fcntl((*connfd), F_GETFL, 0);
    // fcntl((*connfd), F_SETFL, flag | O_NONBLOCK);

    pthread_create(&tid, NULL, &doit, connfd);
  }

  return 0;
}

static void *doit(void *arg){
  int connfd = *( (int*) arg);
  free(arg);
  pthread_detach(pthread_self());
  str_echo(connfd);
  close(connfd);
}
