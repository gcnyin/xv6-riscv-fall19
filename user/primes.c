#include "kernel/types.h"
#include "user/user.h"

int child(int read_fd) {
  int tmp;
  int intial_prime = 0;
  int fd[2];
  pipe(fd);
  int flag = 0;
  while (read(read_fd, &tmp, sizeof(tmp))) {
    if (!intial_prime) {
      intial_prime = tmp;
      printf("prime %d\n", intial_prime);
    }
    if (tmp % intial_prime == 0) {
      continue;
    }
    if (!flag) {
      if (fork() == 0) {
        close(fd[1]);
        child(fd[0]);
      } else {
        close(fd[0]);
        flag = 1;
      }
    }
    write(fd[1], &tmp, sizeof(tmp));
  }
  close(fd[1]);
  wait();
  exit();
}

int main() {
  printf("prime 2\n");
  int fd[2];
  int flag = 0;
  int tmp;
  pipe(fd);
  for (int i = 2; i < 35; i++) {
    if (i % 2 == 0) {
      continue;
    }
    tmp = i;
    if (!flag) {
      if (fork() == 0) {
        close(fd[1]);
        child(fd[0]);
      } else {
        close(fd[0]);
        flag = 1;
      }
    }
    write(fd[1], &tmp, sizeof(tmp));
  }
  close(fd[1]);
  wait();
  exit();
}