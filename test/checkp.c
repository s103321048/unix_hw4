#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

int path_check(const char *path){
//    char *now = getenv("path_now");
    char *now = "/Desktop";
    printf("root now: %s" , now);
    char root[1024];
    realpath(now, root);
    char trg_path[1024];
    realpath(path, trg_path);
    if (strncmp(trg_path, root, strlen(root)) != 0){ 
        fprintf( stderr,"ls: cannot open directory '%s'\n", path);
        return 0;
    }   
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2)
       fprintf(stderr, "you need a path\n");
    else{
        if( path_check(argv[1]))
            printf("avalible\n");
    }
}
