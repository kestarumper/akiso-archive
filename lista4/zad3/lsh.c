#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LSH_BUFSIZE 512
#define LSH_TOKEN_DELIMITERS " \t\r\n"

char ** lsh_split(char *, char *);

void ctrl_c_handler(int dummy)
{
  printf("\n");
  printf("> ");
  fflush(STDIN_FILENO);
}

int lsh_inbuilt_cd(char ** args) {
  if(args[1] == NULL) {
    fprintf(stderr, "lsh: cd expected a parameter\n");
  } else {
    if(chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return EXIT_SUCCESS;
}

int lsh_inbuilt_exit(char ** args) {
  exit(EXIT_SUCCESS);
}

int (*lsh_inbuilt_functions[])(char **) = {
  &lsh_inbuilt_cd,
  &lsh_inbuilt_exit
};

char * lsh_inbuilt_names[] = {
  "cd",
  "exit"
};

int lsh_inbuilt_functions_size = (sizeof(lsh_inbuilt_names) / sizeof(char*));

pid_t spawn_process(char ** args, int in, int out, int err)
{
  pid_t pid;
  int status;

  pid = fork();
  if(pid == 0) {
  // fprintf(stdout, "%s: IN[%i] --> OUT[%i]\n", args[0], in, out);
    if(in != STDIN_FILENO) {
      dup2(in, STDIN_FILENO);
      close(in);
    }
    if(out != STDOUT_FILENO) {
      dup2(out, STDOUT_FILENO);
      close(out);
    }
    if(err != STDERR_FILENO) {
      dup2(err, STDERR_FILENO);
      close(err);
    }

    if(execvp(args[0], args) == -1) {
      fprintf(stderr, "lsh:spawn:execvp = %s\n", args[0]);
    }

    exit(EXIT_FAILURE);
  }

  return pid;
}

int lsh_launch(char ** commands, int n, int run_in_bg, int redirect_type, char * redirect_file)
{
  char ** com_args;

  pid_t pid;
  pid_t wpid;
  int status = EXIT_SUCCESS;

  int i = 0;

  // com_args = lsh_split(commands[0], LSH_TOKEN_DELIMITERS);


  int fd[2];
  int in = STDIN_FILENO;
  int out = STDOUT_FILENO;
  int err = STDERR_FILENO;
  for (i = 0; i < n - 1; ++i)
  {
    com_args = lsh_split(commands[i], LSH_TOKEN_DELIMITERS);

    if(pipe(fd)) {
      perror("lsh:launch:pipe");
      return 1;
    }

    // printf("%i zapisuje do %i\n", fd[STDOUT_FILENO], fd[STDIN_FILENO]);

    pid = spawn_process(com_args, in, fd[STDOUT_FILENO], err);
    close(fd[STDOUT_FILENO]);

    in = fd[STDIN_FILENO];
  }

  // Kod ktory zabral mi 8h mojego zycia
  // dup2 byl wykonywany rowniez w funkcji spawn_process
  // co prowadzilo do... nie wiem czego, ale chyba zamykalo pipe'a
  // z ktorego potem szly same EOF'y
  // if (in != STDIN_FILENO) {
  //   dup2(in, STDIN_FILENO);
  // }

  com_args = lsh_split(commands[i], LSH_TOKEN_DELIMITERS);


  // Check if not empty
  if (com_args[0] == NULL) {
    // An empty command was entered.
    return EXIT_SUCCESS;
  }

  for (i = 0; i < lsh_inbuilt_functions_size; i++) {
    if (strcmp(com_args[0], lsh_inbuilt_names[i]) == 0) {
      return (*lsh_inbuilt_functions[i])(com_args);
    }
  }

  if(redirect_type >= 0) {
    switch(redirect_type) {
      case STDOUT_FILENO:
        out = creat(redirect_file, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if(out < 0) {
          perror("lsh:launch:fileopen:out");
          return 1;
        }
        break;
      case STDIN_FILENO:
        in = open(redirect_file, O_RDONLY);
        if(in < 0) {
          perror("lsh:launch:fileopen:in");
          return 1;
        }
        break;
      case STDERR_FILENO:
        printf("STDERR to %s\n", redirect_file);
        err = creat(redirect_file, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if(out < 0) {
          perror("lsh:launch:fileopen:err");
          return 1;
        }
        break;
    }
  }
  
  pid = spawn_process(com_args, in, out, err);

  // wait (or dont) for process
  if(!run_in_bg) {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while(!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

char * lsh_readline()
{
  int bufsize = LSH_BUFSIZE;
  char * buffer = malloc(sizeof(char) * bufsize);
  int c;
  int position = 0;

  if(!buffer) {
    perror("lsh:readline:malloc");
    exit(EXIT_FAILURE);
  }

  while(1) {
    c = getchar();

    if(position == 0 && c == EOF) {
      printf("\nBye :)\n");
      exit(EXIT_SUCCESS);
    } else
     if(c == '\n' || c == EOF) {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }

    position++;
  }
}

char ** lsh_split(char * line, char * delims)
{
  int bufsize = LSH_BUFSIZE;
  char * token;
  char ** tokens = malloc(sizeof(char*) * bufsize);
  char ** tokens_temp;

  if(!tokens) {
    perror("lsh:split:malloc");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, delims);
  int pos = 0;
  while(token != NULL) {
    tokens[pos] = token;
    pos++;

    if(pos >= bufsize) {
      tokens_temp = tokens;
      bufsize += LSH_BUFSIZE;
      tokens = realloc(tokens, sizeof(char*) * bufsize);

      if(!tokens) {
        perror("lsh:split:realloc");
        free(tokens_temp);
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, delims);
  }
  // add NULL at the end to signal last element
  tokens[pos] = NULL;

  return tokens;
}

void lsh_loop()
{
  char * line;
  char ** commands;
  int status;
  int comnum;
  int run_in_bg;

  char * redirect_file;
  int redirect_type = -1;

  char * ampersand_pos = NULL;
  char * redirect_pos = NULL;

  do {
    redirect_type = -1; // -1 means no redirect
    run_in_bg = 0;
    ampersand_pos = NULL;
    redirect_pos = NULL;

    printf("> ");

    line = lsh_readline();

    if(ampersand_pos = strchr(line, '&')) {
      // remove ampersand from args and replace it with null byte
      *ampersand_pos = '\0';
      run_in_bg = 1;
    }

    if(redirect_pos = strchr(line, '<')) {
      redirect_type = STDIN_FILENO;
      *redirect_pos = '\0';
      redirect_file = redirect_pos+1;
      while(*redirect_file == ' ') { 
        redirect_file++;
      }
    }

    if(redirect_pos = strchr(line, '2')) {
      if(*(redirect_pos+1) == '>') {
        redirect_type = STDERR_FILENO;
        *redirect_pos = '\0';
        *(redirect_pos+1) = '\0';
        redirect_file = redirect_pos+2;
        while(*redirect_file == ' ') { 
          redirect_file++;
        }
      }
    } else if(redirect_pos = strchr(line, '>')) {
      redirect_type = STDOUT_FILENO;
      *redirect_pos = '\0';
      redirect_file = redirect_pos+1;
      while(*redirect_file == ' ') { 
        redirect_file++;
      }
    }

    commands = lsh_split(line, "|");

    for(comnum = 0; commands[comnum] != NULL; comnum++);

    status = lsh_launch(commands, comnum, run_in_bg, redirect_type, redirect_file);

    free(line);
    // for(int i = 0; commands[i] != NULL; i++)
    //   free(commands[i]);

  } while(1);
}

int main() 
{
  printf("LSH Alpha\n");

  // handle CTRL+C
  signal(SIGINT, ctrl_c_handler);

  // remove zombies
  struct sigaction sigchld_action = {
    .sa_handler = SIG_DFL,
    .sa_flags = SA_NOCLDWAIT
  };
  sigaction(SIGCHLD, &sigchld_action, NULL);

  lsh_loop();

  return 0;
}