#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char *envp[])
{
   printf("Content-type: text/html\n\n");
   char *query = getenv("QUERY_STRING");
   char name[20];
   int age;
   sscanf(query, "name=%[^&]&age=%d", name, &age);
   printf("name:%s<br/>age:%d", name, age);
}

