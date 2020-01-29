#include "kernel/types.h"
#include "user/user.h"

int main() {
  int parent_fd[2];
  int child_fd[2];
  pipe(parent_fd);
  pipe(child_fd);
  if (fork() == 0) {
    char p[1];
    close(parent_fd[1]);
    close(child_fd[0]);
    read(parent_fd[0], p, 1);
    close(parent_fd[0]);
    printf("%d: received ping\n", getpid());
    write(child_fd[1], "c", 1);
    close(child_fd[1]);
    exit();
  }
  close(parent_fd[0]);
  close(child_fd[1]);
  write(parent_fd[1], "p", 1);
  close(parent_fd[1]);
  char main_p[1];
  read(child_fd[0], main_p, 1);
  printf("%d: received pong\n", getpid());
  close(child_fd[0]);
  exit();
}