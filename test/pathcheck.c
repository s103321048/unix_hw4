#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(){
    struct stat sb;
    int ret;

    ret = stat("/Users/dragonfly/Desktop/hw4/dir2", &sb);
    if (ret){
        perror("stat");
        return 1;
    }

    if (S_ISDIR(sb.st_mode)) {
        puts("debug is a dir.\n");
    } else{
        puts("debug is a file.\n");
    }
    return 0;
}
