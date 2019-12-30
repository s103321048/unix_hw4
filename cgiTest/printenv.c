#include <stdio.h>

int main(int argc, char* argv[], char *envp[])
{
   printf("Content-type: text/html\n\n");
   int i;
   char *query;
   for(i = 0; envp[i] != NULL; ++i ){
      printf("%s<br/>", envp[i]);
   }
}
