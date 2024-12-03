#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "ioctl.h"

int main()
{
	int fd;
	fd = open("/dev/Shiva", O_RDWR);
	if(fd == -1)
	{
		perror("Error in opening device file\n");
		return -1;
	}
	printf("fd = %d\n", fd);
	ioctl(fd, CMD_1);
    sleep(10);
	ioctl(fd, CMD_2, 45);
	close(fd);

	return 0;
}
