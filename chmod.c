#include "types.h"
#include "user.h"

uint
octal(char* s)
{
  uint o = 0;
  for (int i = 0; i < strlen(s); i++) {
    o <<= 3;
    o += s[i] - '0';
  }
  return o;
}

int
main(int argc, char* argv[])
{
  if (argc != 3) printf(2, "Wrong number of arguments\n");
  else if (octal(argv[1]) > 01777) printf(2, "Invalid mode\n");
  else {
    if (chmod(argv[2], octal(argv[1]))) {
      printf(2, "Chmod failed\n");
    }
  }
  exit();
}