#include <unistd.h>
#include <stdio.h>

int main()
{
    if(access("../testcase/dir1/index.html", R_OK) == 0)
    printf("/etc/passwd can be read\n");
}

