#include <stdio.h>
#include <string.h>

void write_something(char *foo);
char * onlyPart(char *foo);
void getGET_line(char *buf);

int main(){
    char print_this[1024] = "Empty\nEmtpy again";
    getGET_line( print_this );
    write_something( print_this);
    printf("print:\n %s\n", print_this);
}

void write_something(char *foo){
    //strcat(foo, "Writing something here");
    strcpy( foo, "second try");
}


void getGET_line(char *buf){
    char *target;
    target = strtok(buf, "\n");
    printf("target = %s\n", target);
}
