#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int isspace(char c) { return c == ' ' || c == '\n' || c == '\t'; }

char *ltrim(char *s) {
  while (isspace(*s)) s++;
  return s;
}

char *rtrim(char *s) {
  char *back = s + strlen(s);
  while (isspace(*--back))
    ;
  *(back + 1) = '\0';
  return s;
}

char *trim(char *s) { return rtrim(ltrim(s)); }

char *fmtname(char *path) {
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if (strlen(p) >= DIRSIZ) return p;
  memmove(buf, p, strlen(p));
  memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
  return buf;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("can not solve parameters");
    exit();
  }
  char *path = argv[1];

  int fd;
  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "can not open path: %s\n", path);
    exit();
  }

  struct stat st;
  if (fstat(fd, &st) < 0) {
    fprintf(2, "can not stat path: %s\n", path);
    close(fd);
    exit();
  }

  char buf[512], *p;
  struct dirent de;
  char *target_name = argv[2];

  if (st.type == T_FILE && st.type == T_DEVICE) {
    printf("can not find file or device");
    exit();
  }
  // st.type == T_DIR
  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';
  char *file_name;
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0) continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if (stat(buf, &st) < 0) {
      printf("ls: cannot stat %s\n", buf);
      continue;
    }
    file_name = trim(fmtname(buf));
    if ((strcmp(file_name, target_name) == 0)) {
      printf("%s\n", trim(buf));
    }
  }
  exit();
}