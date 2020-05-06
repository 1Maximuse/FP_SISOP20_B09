#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"
#include "fs.h"

char*
filename(char* path)
{
  int lastslash = -1;
  for (int i = 0; i < strlen(path); i++) {
    if (path[i] == '/') lastslash = i;
  }
  lastslash++;
  return path+lastslash;
}

int
main(int argc, char* argv[])
{
  int dst = open(argv[2], O_RDONLY);
  struct stat st;
  fstat(dst, &st);
  if (st.type != T_DIR) {
    printf(2, "Error, destination is not a folder.\n");
    exit();
  }
  close(dst);
  if (argv[1][strlen(argv[1])-1] == '*' && (strlen(argv[1]) == 1 || argv[1][strlen(argv[1])-2] == '/')) {
    char folderpath[strlen(argv[1])];
    strcpy(folderpath, argv[1]);
    folderpath[strlen(folderpath) - 1] = '\0';
    if (strlen(folderpath) == 0) strcpy(folderpath, "/");
    int folder = open(folderpath, O_RDONLY);
    if (folder < 0) {
      printf(2, "Cannot open source folder.\n");
      exit();
    }
    struct dirent de;
    while ((read(folder, &de, sizeof(de)) == sizeof(de))) {
      char filepath[strlen(folderpath) + strlen(de.name) + 10];
      strcpy(filepath, folderpath);
      if (filepath[strlen(filepath) - 1] != '/') {
        strcpy(filepath + strlen(filepath), "/");
      }
      strcpy(filepath + strlen(filepath), de.name);
      int src = open(filepath, O_RDONLY);
      struct stat st;
      fstat(src, &st);
      if (st.type == T_DIR) continue;

      char destfile[strlen(argv[2]) + strlen(folderpath) + 10];
      strcpy(destfile, argv[2]);
      if (destfile[strlen(destfile) - 1] != '/') {
        strcpy(destfile + strlen(destfile), "/");
      }
      strcpy(destfile + strlen(destfile), de.name);
      unlink(destfile);
      int destfd = open(destfile, O_WRONLY | O_CREATE);

      int len;
      char buf[1024];
      while ((len = read(src, buf, 1024)) > 0) {
        write(destfd, buf, len);
      }
      close(src);
      close(destfd);
      unlink(filepath);
    }
    close(folder);
  } else {
    int src = open(argv[1], O_RDONLY);
    if (src < 0) {
      printf(2, "Cannot open source file.\n");
      exit();
    }
    struct stat st;
    fstat(src, &st);
    if (st.type == T_DIR) {
      printf(2, "Error, source file is directory.\n");
      exit();
    }

    char destfile[strlen(argv[2]) + strlen(argv[1]) + 10];
    strcpy(destfile, argv[2]);
    if (destfile[strlen(destfile) - 1] != '/') {
      strcpy(destfile + strlen(destfile), "/");
    }
    strcpy(destfile + strlen(destfile), filename(argv[1]));
    unlink(destfile);
    int destfd = open(destfile, O_WRONLY | O_CREATE);

    int len;
    char buf[1024];
    while ((len = read(src, buf, 1024)) > 0) {
      write(destfd, buf, len);
    }
    close(src);
    close(destfd);
    unlink(argv[1]);
  }
  exit();
}