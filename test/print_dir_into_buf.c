#include <dirent.h>
#include <stdio.h>
#include <string.h>
int main(){
    DIR *dir;
    struct dirent *ent;
    int count = 0;

    char dirFiles[1024];

    char *PATH = "/Users/dragonfly/Desktop/hw4/testcase";
    if ((dir = opendir (PATH)) != NULL) {
  /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
        printf ("%d, %s\n", count,  ent->d_name);
        count++;

        char *eachfile = ent->d_name;
        strcat( eachfile, "\n");
        strcat( dirFiles, eachfile);

    }
    printf("%s\n", dirFiles);
    closedir (dir);
    } else {
  /* could not open directory */
    perror ("");
    }
}
