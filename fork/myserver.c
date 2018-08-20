#include "my_unp.h"
#include "str_echo.h"

void sig_chld(int);

int main(int argc, char const *argv[]) {
  int listenfd = init(SERVER, NULL);
  int connfd, childpid;
  signal(SIGCHLD, sig_chld);

  for(;;){
      connfd = Accept(listenfd, NULL, NULL);
      if( (childpid = fork()) == 0){
        //子进程处理
        close(listenfd);
        str_echo(connfd);
        exit(0);
      }
      close(connfd);
  }
  return 0;
}

void sig_chld(int signo){
  pid_t pid;
  int stat;
  while( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
    // printf("child %d terminated\n", pid);
  return;
}
