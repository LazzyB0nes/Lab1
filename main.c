#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

void rar(char *dir)
{
	DIR *dp;
	const struct dirent *entry;
	const struct stat statbuf;
	char block[1024] = {0};
	int output, in, out;
	int size, nread, quantity = 0;

	dp = opendir(dir);

	if (dp == NULL) 
	{
		fprintf(stderr, "cannot open directory: %s\n", dir);
		return;
	}
	
	chdir(dir);
	output = open("logfile.out", O_WRONLY | O_CREAT, 0600);
	while ((entry = readdir(dp)) != NULL) 
	{
		lstat(entry->d_name, &statbuf);
		in = open(entry->d_name, O_RDONLY);

		if (S_ISDIR(statbuf.st_mode)) 
		{
			if ((strcmp(".", entry->d_name)) == 0
			|| (strcmp("..", entry->d_name)) == 0)
				continue;
			continue;
		}
		else
		{
			if ((strcmp("logfile.out", entry->d_name)) != 0) 
			{
				quantity++;
				write(output, entry->d_name, strlen(entry->d_name));
				write(output, " ", 1);
				write(output, &(statbuf.st_size), sizeof(statbuf.st_size));
				write(output, "\n", 1);

				while ((nread = read(in, block, sizeof(block))) > 0)
				write(output, block, nread);
				write(output, "\n", 1);
			}
		}	
	}
	
	write(output,&(quantity),sizeof(quantity));	
	closedir(dp);
}
void unrar(void)
{
	char block[1024] = {0};
	char block1[1024] = {0};
	int output, in;
	int  nread = 0, rread = 0, quantity = 0;
	long i = 0, size = 0;

	in = open("logfile.out", O_RDONLY);
	if (in == -1)
	{
		printf("1open error");
	}
	lseek(in, -4, SEEK_END);
	read(in,&quantity,4);
	lseek(in, 0, SEEK_SET);
	for (int a = 0; a < quantity; a++) 
	{
		i = 0;
		while (strcmp(block, " ") != 0) 
		{
			read(in, block, 1);
 //    		if (strcmp(block, " ") == 0)
 //         	break;
			block1[i] = block[0];
			i++;
		}
		read(in,&size,8);
		read(in, block, 1);
		output = open(block1, O_WRONLY | O_CREAT, 0600);
		
		if (size > sizeof(block))
		{
			rread = size / sizeof(block);
			nread = size - (rread * sizeof(block));

			for (i = 1; i <= rread; i++)
			{
				read(in, block, sizeof(block));
				write(output, block, sizeof(block));
			}
			read(in,block,nread);
			write(output,block,nread);
		}
		else
		{
			read(in,block,size);
			write(output,block,size);
		}
		read(in,block,1);
		memset(block, 0, sizeof(block));
		memset(block1, 0, sizeof(block1));
		size = 0; nread = 0; rread = 0;
	}
}
void main(void)
{
	int i = 0;
	char block[120] = {0};
	char block1[120] = {0};
	write(1, "Directory scan of:", 19);
	read(0, block, 120);

    while (block[i] != '\n') 
	{
        block1[i] = block[i];
    	i++;
	}

	write(1, block1, strlen(block));
    rar(block1);
    unrar();
}