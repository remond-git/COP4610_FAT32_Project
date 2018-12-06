#include <stdio.h>
#include "../include/FATImage.h"
#include "../include/filetable.h"
#include <dirent.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#define TOK_DELIMS " \t\r\n\a"
#define TOK_BUFF 64

/*

exit: 5 pts
info: 5 pts
ls: 5 pts
cd: 5 pts
size: 5 pts
creat: 5 pts
mkdir: 10 pts
open: 5 pts
close: 5 pts
read: 10 pts
write: 10 pts
Extra Credit: rm, 5 pts
Extra Credit: rmdir, 5 pts

*/

int num_builtins() {
  return sizeof(builtin) / sizeof(char *);
}

int shell_cd(char **args);
int shell_exit(char **args);
int shell_info(char **args);
int shell_ls(char **args);
int shell_size(char **args);
int shell_creat(char **args);
int shell_mkdir(char **args);
int shell_open(char **args);
int shell_close(char **args);
int shell_read(char **args);
int shell_write(char **args);
int shell_rm(char **args);
int shell_rmdir(char **args);

char* current_path();


char *builtin[] = {
  "cd",
  "exit",
  "info",
  "ls",
  "size",
  "creat",
  "mkdir",
  "open",
  "close",
  "read",
  "write",
  "rm",
  "rmdir"
};

int (*builtin_func[]) (char **) = {
  &shell_cd,
  &shell_exit,
  &shell_info,
  &shell_ls,
  &shell_size,
  &shell_creat,
  &shell_mkdir,
  &shell_open,
  &shell_close,
  &shell_read,
  &shell_write,
  &shell_rm,
  &shell_rmdir
};

/*

==========
MAIN
==========

*/


int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Error: No FAT32 image file supplied.\n");
    } else if (argc > 3) {
        printf("Error: Too many arguments supplied.\n");
    }

    FILE *image = fopen(argv[1], "rb+");

    if (image == NULL) {
        printf("Error: Could not open FAT32 image file.\n");

        return 1;
    } else {
        printf("Success: FAT32 image file successfully opened.\n");
    }

    fseek(image, 3, SEEK_SET);
    char oemName[8];
    fread(oemName, sizeof(char), 8, image);
    printf("OEMName: %s\n", oemName);

    shell_loop();              // Main Loop
    printf("poot");
    return EXIT_SUCCESS;          // Exit
}

/*

PROMPT

*/

static int io_flag = 0;

void fullPrompt() {
  char* img_file = argv[1];
  char* current_path = current_path();


  printf("%s : %s" , img_file , current_path)

}

char* current_path(){
  /*

Code to find pwd...

  */
}

static const char prompt[] = "";

struct timespec start, stop;

static int args_size = 0;

int shell_launch(char **args) {
  pid_t p, wp;
  FILE* io_stat;
  char fbuff[300];
  int st, fd0, fd1, i, fd[2];
  int bg_flag = 0;
  char** subset;
  char buf;
  char caten[30];
  int buffer_status;
  p = fork();
  //Child
  if(p == 0) {
    for(i = 0; args[i] != '\0'; i++) {
	  //Input
      if(strcmp(args[i],"<") == 0) {
	if(args[i+1] == NULL) {
	  fprintf(stderr, "Missing name for redirect\n");
	  exit(0);
        }
	else {
	  if((fd0 = open(args[i+1], O_RDONLY)) < 0) {
            perror("cant open file");
            exit(0);
          }
        }
	dup2(fd0, 0);
	close(fd0);
      }
	  //Output
      if(strcmp(args[i],">") == 0) {
	if(args[i+1] == NULL) {
          fprintf(stderr, "Missing name for redirect\n");
          exit(0);
        }
	else {
          if((fd1 = open(args[i+1], O_WRONLY | O_CREAT, 0644)) < 0) {
	    perror("cant open file\n");
	    exit(0);
	  }
	}
	dup2(fd1, STDOUT_FILENO);
        close(fd1);
      }
	  //Pipe
      if(strcmp(args[i],"|") == 0) {
	if(args[i+1] == NULL || i == 0) {
          fprintf(stderr, "Invalid null command\n");
          exit(0);
        }
	pipe(fd);
	if((p = fork()) == 0) {
	  close(1);
	  dup2(fd[1], 1);
	  close(fd[0]);
	  close(fd[1]);
	  execv(args[i-1], args);
        }
	else {
	  close(0);
	  dup2(fd[0], 0);
	  close(fd[0]);
	  close(fd[1]);
	  execv(args[i+1], args);
	}
      }
      if((strcmp(args[i],"io") == 0) && (i == 0)) {
	    io_flag = 1;
      }
      if(strcmp(args[i], "&") == 0) {
	if(i != 0 && i == sizeof(args-1)) {
      	  bg_flag = 1;
	  break;
	}
      }
    }
    if(io_flag == 0) {
      if(execv(args[0], args) == -1) {
        perror("shell");
      }
      exit(EXIT_FAILURE);
    }
    else {
      subset = &args[1];
      if(execv(args[1], subset) == -1) {
	perror("shell");
      }
      exit(EXIT_FAILURE);
    }
  }
  //Error
  else if(p < 0) {
    perror("shell");
  }
  //Parent
  else {
    //if(!bg_flag) {
	  bg_flag = 0;
	  for(i = 0; args[i] != '\0'; i++) {
		  //Check if this is supposed to execute in the BG
		  if(strcmp(args[i], "&") == 0) {
		    if(i != 0 && i ==(args_size - 1)) {
			  bg_flag = 1;
			  break;
		    }
		    else if (i != 0) {
			  bg_flag = -1;
			  break;
		    }
		  }
	  }
	  //Foreground Execution
	  if(bg_flag == 0) {
        do {
          wp = waitpid(p, &st, WUNTRACED);
        } while(!WIFEXITED(st) && WIFSIGNALED(st));
        if(io_flag == 1) {
      	  sprintf(caten, "/proc/%d/io", (int)wp);
	      printf(caten); printf("\n");
		  io_stat = fopen(caten, "r");
		  int x = 0;
		  char c;
		  if(io_stat == NULL) {
			perror("Failed: ");
		  }
		  else {
		    while((c=fgetc(io_stat)) != EOF) {
			  fbuff[x++] = c;
		    }
		    fbuff[x] = '\0';
		    printf(fbuff);
	         }
        }
         }
	  //Background Execution
	  else if(bg_flag == 1) {
		  wp = waitpid(-1,&st, WNOHANG);
		  printf("Background pid: %d\n", p);
		  if(wp > 0) {
			 printf("Completed Background pid: %d\n", wp);
		  }
	  }
	  else if(bg_flag == -1)
		  printf("Invalid Command\n");
	}
  /*wp = waitpid(-1, &st, WNOHANG);
  while (wp > 0) {
    printf("completed bg pid: %d\n", wp);
  }*/
  return 1;
}

char * tok_prep(char * line) {
  int pos = 0;
  while(pos != strlen(line) - 1) {
    if(line[pos] == ' ' || line[pos] == '\t') {
      line[pos] = '\a';
    }
    else if(line[pos] == '\'') {
      line[pos] = '\a';
      while(line[pos] != '\'') {
	pos++;
      }
      line[pos] = '\a';
    }
    pos++;
  }
  return line;
}

char **shell_split(char *line) {
  if(line[0] == NULL) {
    line[0] = "";
  }
  char * _line = tok_prep(line);
  int buff = TOK_BUFF;
  int position = 0;
  char **tokens = malloc(buff * sizeof(char*));
  char *token;
  if(!tokens) {
    fprintf(stderr, "shell: alloc error\n");
    exit(EXIT_FAILURE);
  }
  token = strtok(_line, "\a");
  while(token != NULL) {
    char * temp;
    temp = token;
    tokens[position] = temp;
    position++;
    if(position >= buff) {
      buff += TOK_BUFF;
      tokens = realloc(tokens, buff * sizeof(char*));
      if(!tokens) {
       fprintf(stderr, "shell: alloc error\n");
       exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, "\a");
  }
  tokens[position] = NULL;
  args_size = position;
  return tokens;
}

/*


==========
MAIN LOOP
==========


*/

void shell_loop() {
  char * line;
  char ** args;
  int status;
  clock_gettime(CLOCK_REALTIME, &start);
  do {
    fullPrompt();
    line = readline(prompt);
    args = shell_split(line);
    int i = 0;
    status = shell_execute(args);

    free(line);
    free(args);
  } while(status);
}

/*

=============
=============
Built In's
=============
=============


*/

/*

BUILT IN: CD

*/

int shell_cd(char **args) {
  if(args[1] == NULL) {
    chdir(getenv("HOME"));
  }
  else {
    if(args[2] != NULL) {
      fprintf(stderr, "cd: Too many arguments\n");
    }
    else if(chdir(args[1]) != 0) {
      perror("cd");
    }
  }
  return 1;
}

/*

BUILT IN: LS

*/

int shell_ls(char **args) {
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if(args[1] == NULL) {
    if(d) {
      while((dir = readdir(d))) {
        printf("%s  ", dir->d_name);
      }
      printf("\n");
      closedir(d);
    }
    else
      perror("Could not open directory");
  }
  else {

    d = opendir(args[1]);
    if(d) {
      while((dir = readdir(d))) {
        printf("%s  ", dir->d_name);
      }
      printf("\n");
      closedir(d);
    }
    else
      perror("Could not open directory");
  }
  return 1;
}


/*

BUILT IN: EXIT

*/

int shell_exit(char **args) {
  clock_gettime(CLOCK_REALTIME, &stop);
  double elapsed2 = (stop.tv_sec - start.tv_sec);
  printf("Exiting...\n\tSession time: %ds \n", (int)elapsed2);
  if(stop.tv_sec != 0)
  //return 0;
    exit(EXIT_SUCCESS);
}


int shell_execute(char **args) {
  int i;
  if(args[0] == NULL)
    return 1;
  for(i = 0; i < num_builtins(); i++) {
    if(strcmp(args[0], builtin[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  return shell_launch(args);
}
