#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

void main(void) 
{ 
  char block[1024];
  int in, out;
  int nread;

  write(1, "Now, i want to copy some files..\n", 33);

  in = open("file.in", O_RDONLY);
  out = open("file.out", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
  while((nread = read(in, block, sizeof(block))) > 0)
    write(out, block, nread);
}
