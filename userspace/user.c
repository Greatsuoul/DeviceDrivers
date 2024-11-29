#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    char buffer[40];

    fd = open("/dev/abcd", O_RDWR);

    const char *write_data = "Hello Worldd!";
    ssize_t bytes_written = write(fd, write_data, strlen(write_data));
   
    printf("Written to device: %s\n", write_data);

    
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    
    buffer[bytes_read] = '\0'; 
    printf("Read from device: %s\n", buffer);

    
    close(fd);
    return 0;
}
