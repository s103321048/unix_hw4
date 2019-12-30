#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>
#define MAX_LEN 1024
int main(){
  char buffer[MAX_LEN+1] = {0};
  int out_pipe[2];
  int saved_stdout;

  char *path = "/Users/dragonfly/Desktop/hw4/testcase/printenv_sh.cgi";
  char command[100];
//    strcpy( command, "ls -l" );
//    system(command);

  saved_stdout = dup(STDOUT_FILENO);  /* save stdout for display later */

  if( pipe(out_pipe) != 0 ) {          /* make a pipe */
    exit(1);
  }

  dup2(out_pipe[1], STDOUT_FILENO);   /* redirect stdout to the pipe */

  system(path);    
  close(out_pipe[1]);

  /* anything sent to printf should now go down the pipe */
  printf("END OF PIPE\n");
  fflush(stdout);

  read(out_pipe[0], buffer, MAX_LEN); /* read from pipe into buffer */

  dup2(saved_stdout, STDOUT_FILENO);  /* reconnect stdout for testing */
  printf("read: %s\n", buffer);
  printf("this is the end\n");
  return 0;
}
