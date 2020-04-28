#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "fs.h"

int
get_inode(char* path)
{
  struct stat st;
  int fd;

  fd = open(path, O_RDONLY);
  if (fstat(fd, &st) < 0) {
    printf(2, "pwd: cannot stat %s\n", path);
    exit();
  }
  close(fd);
  return st.ino;
}

void
get_dirent_from_inode(char* path, int inode, struct dirent* ent)
{
  struct dirent d;
  int fd;
  fd = open(path, O_RDONLY);
  while (read(fd, &d, sizeof(d)) == sizeof(d)) {
    if (d.inum == inode) {
      break;
    }
  }
  close(fd);
  *ent = d;
}

int
main(int argc, char* argv[])
{
  char current[512] = ".";
  char parent[512] = "..";
  char pwd[512] = "";

  while (get_inode(current) != get_inode(parent)) {
    struct dirent ent;
    get_dirent_from_inode(parent, get_inode(current), &ent);
    char name[512];
    strcpy(name, pwd);
    strcpy(name + strlen(pwd), "/");
    strcpy(name + strlen(pwd) + 1, ent.name);
    strcpy(pwd, name);
    strcpy(current, parent);
    strcpy(name, "../");
    strcpy(name + 3, parent);
    strcpy(parent, name);
  }
  if (pwd[0] == '\0') {
    pwd[0] = '/';
    pwd[1] = '\0';
  }
  printf(0, "%s\n", pwd);
  exit();
}
