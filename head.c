#include "types.h"
#include "user.h"
#include "fcntl.h"

int
toint(char* s)
{
  int res = 0, neg = 0;
  int i = 0;
  if (s[i] == '-') {
    neg = 1;
    i++;
  }
  for (; i < strlen(s); i++) {
    res = (res << 3) + (res << 1);
    res += s[i] - '0';
  }
  if (neg) res *= -1;
  return res;
}

int
main(int argc, char* argv[])
{
  int verbose = 0;
  int quiet = 0;
  int files = 0;
  int bytes = -1;
  int lines = 10;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (argv[i][1] == 'v') {
        verbose = 1;
      } else if (argv[i][1] == 'c') {
        i++;
        bytes = toint(argv[i]);
      } else if (argv[i][1] == 'q') {
        quiet = 1;
      } else if (argv[i][1] == 'n') {
        i++;
        lines = toint(argv[i]);
      }
    } else {
      files = i;
      break;
    }
  }
  if (files < argc-1 && quiet == 0) {
    verbose = 1;
  }
  int pertama = 1;
  for (int i = files; i < argc; i++) {
    int fd = open(argv[i], O_RDONLY);
    if (verbose) {
      if (!pertama) printf(1, "\n");
      printf(1, "==> %s <==\n", argv[i]);
    }
    pertama = 0;
    char buf;
    int times = 0;
    if (bytes == -1) {
      while (read(fd, &buf, 1) > 0) {
        printf(1, "%c", buf);
        if (buf == '\n') times++;
        if (times == lines) break;
      }
    } else {
      while (read(fd, &buf, 1) > 0) {
        printf(1, "%c", buf);
        times++;
        if (times == bytes) break;
      }
    }
    close(fd);
  }
  exit();
}