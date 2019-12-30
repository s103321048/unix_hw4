#include <dirent.h>
#include <stdio.h>
#include <string.h>
int main(){
    DIR *dir;
    struct dirent *ent;


    char *PATH = "/Users/dragonfly/Desktop/hw4/testcase";
    if ((dir = opendir (PATH)) != NULL) {
  /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
        printf ("%s\n", ent->d_name);
    }
    closedir (dir);
    } else {
  /* could not open directory */
    perror ("");
    }
}
