#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(){
    char buffer[80];
    int fd, size;
    char *path = "/Users/dragonfly/Desktop/hw4/testcase/plain.txt";
    fd = open( path , O_RDONLY);

    size = read(fd, buffer, sizeof(buffer));
    close(fd);
    printf("%s\n", buffer);
}

