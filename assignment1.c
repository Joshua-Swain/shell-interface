#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 80 /* The maximum length command */

int main(void)
{
  char *args[MAX_LINE/2+1]; /* command line arguments */
  int should_run = 1; /* flag to determine when to exit the program */

  while (should_run){
    print("CSCI3120>");
    fflush(stdout);

    /**
    * Take m input and keep in list/array with number
    * Parse input -> error handling
    * Look for "&" in the input
    *
    *
    * 1. After reading input, fork child process using fork() -> must run concurrently
    * 2. Child process will invoke execvp()
    * 3. If command included "&", parent will invoke wait()
    *
    *
    * Then:
    * Make history()
    * Make !! function
    * Make !N function
    */
  }
}
