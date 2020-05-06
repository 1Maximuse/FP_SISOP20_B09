#include "types.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char* argv[])
{
  int src = open(argv[1], O_RDONLY);
  int dst = open(argv[2], O_WRONLY | O_CREATE);
  if (src < 0) {
    printf(2, "Cannot open source file.\n");
    exit();
  }
  if (dst < 0) {
    printf(2, "Cannot open destination file.\n");
    exit();
  }
  int len;
  char buf[1024];
  while ((len = read(src, buf, 1024)) > 0) {
    write(dst, buf, len);
  }
  close(src);
  close(dst);
  unlink(argv[1]);
  exit();
}