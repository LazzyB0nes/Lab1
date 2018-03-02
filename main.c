#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

void meta(char *dir)
{
DIR *dp;
const struct dirent *entry;
const struct stat statbuf;
char block[1024] = {0};
int output, in, out, nread;
int size;

dp = opendir(dir);

if (dp == NULL) {
fprintf(stderr, "cannot open directory: %s\n", dir);
return;
}

chdir(dir);
output = open("logfile.out", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

while ((entry = readdir(dp)) != NULL) {
lstat(entry->d_name, &statbuf);
in = open(entry->d_name, O_RDONLY);

if (S_ISDIR(statbuf.st_mode)) {
if ((strcmp(".", entry->d_name)) == 0
|| (strcmp("..", entry->d_name)) == 0) continue;
continue;
//write(output,entry->d_name,strlen(entry->d_name));
//write(output, " ", 1);
//meta(entry->d_name);  
}

else {
if ((strcmp("logfile.out", entry->d_name)) != 0) {
write(output, "|", 1);
write(output, entry->d_name, strlen(entry->d_name));
write(output, " ", 1);
write(output, "\n", 1);
while ((nread = read(in, block, sizeof(block))) > 0) {
write(output, block, nread);
size += nread;
}
write(output, size, size);.
write(output, "\n", 1);
}
}
}
chdir("..");
closedir(dp);
}

void main(void)
{
write(1, "Directory scan of:", 19);
write(1, "/home/lazzybones/cpp\n", 21);
meta("/home/lazzybones/cpp");
}
