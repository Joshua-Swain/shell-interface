#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 80 /* The maximum length command */

int main(void)
{
  char *args[MAX_LINE/2+1]; /* command line arguments */
  int should_run = 1; /* flag to determine when to exit the program */
  int i = 0;
  int word_length = 0;
  int cursor = 0;
  int no_more_args = 0;
  int arg_count = 0;

  while (should_run){
    printf("CSCI3120>");
    fflush(stdout);
    memset(args, '\0', sizeof(args));

    // read line and store as char array
    char line[640];
    char *cmd = fgets(line, 640, stdin);
    
    if (!cmd) {
      printf("Line not read.\n");
    }
    else {
      printf("Line was read.\n");
      no_more_args = 0;
      arg_count = 0;
      cursor = 0;
      
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
      
      // Print the args array
      for (i=0; i<(sizeof(args) / 8); i++) {
        if (args[i] == '\0') {
          break;
        }
        printf("%s | ", args[i]);
      }
      printf("\n");
    }
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
