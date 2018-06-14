#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>

#define MAX_LINE 80 /* The maximum length command */
#define HIST_NUM 10 /*number of history*/

struct hist
{
  int id;
  char cmd[MAX_LINE + 1];
};

static int nid = 1; /* the id for the next command added into the
history */
struct hist records[HIST_NUM]; /* up to NUM_HISTORY records */
/* try to get the history command index from the arg
* ifarg is not a !! or !followed by N, simply return -1. */
int get_history_index(char arg[]);
int fetch_history(int index, char command[]);
void addToHistory(char line[]); //add command to the history
void display();//displays the history
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
 * Reads a command and places it in an array of strings
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

// Get the command of the given index, if failed return 0.
//otherwise, return 1.
//returns the most recent command if index =0
//otherwise, returns the command in the given index
int getHistory(int index, char cmd[])
{
  int i;
  if (nid == 1)
  {
    return 0; //no command added
  }
  if (index == 0)
  {
    index = nid - 1; //get the most recent
  }

  for (i = HIST_NUM - 1; i >= 0; i--)
  {
    if (records[i].id == index)
    {
    //Get the command in the given index then copy the command
    strcpy(cmd, records[i].cmd);
    return 1;
    }
  }
return 0; //no command with the index, failed
}

//add the line of command into history
void addToHistory(char line[])
{
  int i;
  //shift all history to the left
  for (i = 0; i < HIST_NUM - 1; i++)
  {
    records[i] = records[i + 1];
  }
 //save the line to the last position in the records array
 records[HIST_NUM - 1].id = nid++;
 strcpy(records[HIST_NUM - 1].cmd, line);
}

//display commands in history
void display()
{
  int i;
  //displays each command in the history
  for (i = HIST_NUM- 1; i >= 0 && records[i].id > 0; i--)
  {
    printf("%3d %s\n", records[i].id, records[i].cmd);
  }
}
