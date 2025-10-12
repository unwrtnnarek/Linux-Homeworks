#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void check(bool arg) {
  if(arg) {
    perror("Fail");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char** argv)
{
  check(argc != 2);

  int fd = open(argv[1], O_WRONLY | O_CREAT, 0664);
  check(fd == -1);

  int fd2 = dup(fd);
  check(fd2 == -1);

  char str[] = "first\n";
  char str2[] = "second";

  write(fd, str, 6);
  write(fd2, str2, 6);


  close(fd);
  close(fd2);

  return 0;
}
