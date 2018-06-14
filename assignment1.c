#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>

#define MAX_LINE 80 /* The maximum length command */

void readline(char * args []);
int argc;

int main(void)
{
  char *args[MAX_LINE/2+1]; /* command line arguments */
  int should_run = 1; /* flag to determine when to exit the program */
  int i = 0;

  while (should_run){
    printf("CSCI3120>");
    fflush(stdout);

    memset(args, '\0', sizeof(args));

    readline(args);

    int background = 0; //for executing in the background

    /*
    *
    * After reading user input, the steps are:
    * (1) fork a child process using fork()
    * (2) the child process will invoke execvp()
    * (3) if command included &, parent will invoke wait()
    */

    //forks a child process
   	pid_t pid = fork();
   	if (pid < 0)
   	{
      fprintf(stderr, "Fork Failed!");
   	}
   	else if (pid > 0) // parent process
   	{
      if (!background) //if not background, wait's until child is done
     	{
        wait(NULL);
     	}
   	}
   	else // else child executes command
   	{
      if (execvp(args[0], args) < 0)
     	{
        exit(errno);
     	}
   	}
  }
}

/*
 * Reads a command and returns it as an array of strings
 */
void readline(char * args[]) {
  // read line and store as char array
  int i = 0;
  int word_length = 0;
  int cursor = 0;
  int no_more_args = 0;
  int arg_count = 0;
  char line[640];
  char *cmd = fgets(line, ((MAX_LINE/2)+1)*8, stdin);


    while (!no_more_args) {
      // Get length of next word, store word in char []
      word_length = 0;
      while (!(isspace(cmd[cursor++]))) {
        word_length++;
      }

      char word [word_length + 1];
      for (i=0; i<word_length; i++) {
        word[i] = cmd[cursor - word_length - 1 + i];
      }
      word[word_length] = '\0';

      // Put word into args[]
      if (word_length > 0) {
        args[arg_count] = (char *) malloc(word_length + 1);
        strcpy(args[arg_count++], word);
      }

      if (cmd[cursor-1] == '\n') {
        no_more_args = 1;
      }
  }
}
