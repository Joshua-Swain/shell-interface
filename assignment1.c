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

static int histIndex = 1; /* the id for the next command added into the
history */
struct hist history[HIST_NUM]; /* up to NUM_HISTORY records */
/* try to get the history command index from the arg
* ifarg is not a !! or !followed by N, simply return -1. */
int getIndex(char arg[]);
int getHistory(int index, char command[]);
void addToHistory(char line[]); //add command to the history
void display();//displays the history
int readline(char *args[], char *cmd);

int main(void)
{
  char *args[MAX_LINE/2+1]; /* command line arguments */
  int should_run = 1; /* flag to determine when to exit the program */
  int i = 0;
  char line[MAX_LINE*8]; // 640
  int argc;

  while (should_run)
  {
    printf("CSCI3120> ");
    fflush(stdout);

    memset(args, '\0', sizeof(args));
    char *cmd = fgets(line, ((MAX_LINE/2)+1)*8, stdin);

    argc = readline(args, cmd);
    char hist_index_str[sizeof(args[0])];
    int hist_index = 0;
    int index = getIndex(args[0]);

    if (index >= 0)
    {
        if (!getHistory(index, cmd))
        {
          /* fail to fetch the command from history */
          if (index == 0)
          {
            printf("No commands in history.\n");
          }
          else
          {
            printf("No such command in history.\n");
          }
       continue;
       }
       readline(args, cmd);
     }
     addToHistory(cmd);

    // check to see if this is a histroy command (history, !!, !N)
    if (strcmp((const char *)args[0], "history") == 0)
    {
      display();
    }

    else if (strcmp((const char *)args[0], "exit") == 0)
    {
      should_run = 0;
      //continue;
    }
    else
    {
      int background = 0; //for executing in the background
      if (strcmp(args[argc - 1], "&") == 0)
      {
        background = 1;
        args[argc - 1] = NULL;
      }

      //forks a child process
      pid_t pid = fork();
      if (pid < 0)
      {
        fprintf(stderr, "Fork failed!");
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
}

//try to get the history command index from the arg
//ifarg is not a !! or ! followed by N, simply return -1.
int getIndex(char arg[])
{
  if (strcmp(arg, "!!") == 0)
  {
    return 0;
  }
  if (arg[0] == '!' &&arg[1] != '\0')
  {
 /* check if it is followed by an integer */
  int i;

  for (i = 1; arg[i] != '\0'; i++)
  {
    if (!isdigit(arg[i]))
    { /* not a digit */
      return -1;
    }
  }
 /* otherwise, return N */
 return atoi(&arg[1]);
 }
 return -1; /* not a history command */
}

/*
 * Reads a command and saves it as an array of strings
 * Returns arg_count
 */
int readline(char * args[], char* cmd)
{
  // Read line and store as char array
  int i = 0;
  int word_length = 0;
  int cursor = 0;
  int no_more_args = 0;
  int arg_count = 0;


  while (!no_more_args)
  {
    // Get length of next word, store word in char []
    word_length = 0;
    while (!(isspace(cmd[cursor++])))
    {
      word_length++;
    }

    char word [word_length + 1];
    for (i=0; i<word_length; i++)
    {
      word[i] = cmd[cursor - word_length - 1 + i];
    }
    word[word_length] = '\0';

    // Put word into args[]
    if (word_length > 0)
    {
      args[arg_count] = (char *) malloc(word_length + 1);
      strcpy(args[arg_count++], word);
    }

    if (cmd[cursor-1] == '\n')
    {
      no_more_args = 1;
    }
  }
  return arg_count;
}

// Get the command of the given index, if failed return 0.
//otherwise, return 1.
//returns the most recent command if index =0
//otherwise, returns the command in the given index
int getHistory(int index, char cmd[])
{
  int i;
  if (histIndex == 1)
  {
    return 0; //no command added
  }
  if (index == 0)
  {
    index = histIndex - 1; //get the most recent
  }

  for (i = HIST_NUM - 1; i >= 0; i--)
  {
    if (history[i].id == index)
    {
      //Get the command in the given index then copy the command
      strcpy(cmd, history[i].cmd);
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
    history[i] = history[i + 1];
  }
  //save the line to the last position in the records array
  history[HIST_NUM - 1].id = histIndex++;
  strcpy(history[HIST_NUM - 1].cmd, line);
}

//display commands in history
void display()
{
  int i;
  //displays each command in the history
  for (i = HIST_NUM- 1; i >= 0 && history[i].id > 0; i--)
  {
    printf("%3d %s", history[i].id, history[i].cmd);
  }
}
