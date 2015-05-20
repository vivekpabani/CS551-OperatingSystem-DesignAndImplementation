/* 
 * CS 551 Project 1 
 * @authors- 
 * Lakshmi Karle 
 * Vivek Pabani
 * Anirudh Sunkineni
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "calc.h"
#include "parser.h"

#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */

/* Global variables */
extern char **environ;      /* defined in libc */
sigset_t blk;
char prompt[] = "cs551> ";    /* default prompt */
char sbuf[MAXLINE];         /* for composing sprintf messages */
struct list *tsh_list;
/* End global variables */


/* Function prototypes */
void eval(char *cmdline);
void io_redirection(char *cmdline);
int builtin_cmd(char **argv);
void sigint_handler(int sig);

/* Helper routines */
int parseline(const char *cmdline, char **argv); 
void read_profile(void);
void set_home_dir(void);
void emit_my_prompt(void);
void do_cd(char **argv);

/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) 
{
    char c;
    char cmdline[MAXLINE];

    /* Redirect stderr to stdout*/ 
    dup2(1, 2);
    read_profile_file();
    set_home_dir();

    /* Install the signal handlers */
    signal(SIGINT,  sigint_handler);   /* ctrl-c */
    
    tsh_list = list_init();
    load_var(tsh_list);
    
    /* Execute the shell's read/eval loop */
    while (1) {

	/* Read command line */
	emit_my_prompt();
	fflush(stdout);
	
    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin)){
        printf("fgets error");
        store_var(tsh_list);
        exit(1);
    }
	if (feof(stdin)) { /* End of file (ctrl-d) */
	    fflush(stdout);
	    continue;
    }

    eval(cmdline);
        
	fflush(stdout);
    } 

    exit(0); /* control never reaches here */
}


/* 
 * eval - Evaluate the command line that the user has just typed in
 * If this is a builtin command then execute it, otherwise try to 
 * parse it further and execute the command based on the precedence &
 * piping requested
*/
void eval(char *cmdline){
    if(cmdline && cmdline[0] == '\0'){
        return;
    }else if(strcmp(cmdline, "\n")==0){
        return;
    }
    char *argv[MAXARGS];
    int status;
    pid_t pid;
    parseline(cmdline, argv);
    sigemptyset (&blk);
    sigaddset (&blk, SIGCHLD);
    sigaddset (&blk, SIGQUIT);
    sigaddset (&blk, SIGTSTP);
    sigaddset (&blk, SIGTERM);
    sigprocmask (SIG_BLOCK, &blk, NULL);
    if(!builtin_cmd(argv)){
        if(!validate(cmdline)){
            printf("Illegal expression with parentheses\n");
            return;
        }
        struct Queue queues[10]; 
        for(int j=0; j<10; j++){
            Queue queue = createQueue();
            queues[j] = queue;
        }
        parse_cmd(cmdline, queues);
        int num_commands = 0;
        int commands_rem = 0;
        for(int k=0; k<10; k++){
            commands_rem += queues[k].size;
        }
        int old_fds[2];
        int new_fds[2];
        for(int i=9; i>=0; i--){
            char curr_cmd[100];
            memset(curr_cmd, 0, sizeof(curr_cmd));
            strcpy(curr_cmd, queues[i].pop(&queues[i]));
            while(strcmp(curr_cmd, "NOMORE")){
                if(commands_rem>1){
                pipe(new_fds);
                printf("Created a pipe in:%d, out:%d",new_fds[0],new_fds[1]);
                fflush(stdout);
                }
                memset(argv, 0, sizeof(argv));
                parseline(curr_cmd, argv);
                printf("Executing command %s\n", curr_cmd);
                if ((pid = fork ()) == 0)
                {
                     if (num_commands>0){
                         close(0);
                         dup2(old_fds[0], 0);
                         printf("Attached pipe-end:%d to stdin\n", old_fds[0]);
                         close(old_fds[0]);
                         close(old_fds[1]);
                     }
                     if (commands_rem>1){
                         printf("Attached pipe-end:%d to stdout\n", new_fds[1]);
                         close(1);
                         close(new_fds[0]);
                         dup2(new_fds[1], 1);
                         close(new_fds[1]);
                     }
                     dup2(1, 2);
                     if(execvp(argv[0],argv)<0){
                         printf("%s: Command not found.\n", argv[0]);
                         exit(0);
                     }
                }else{
                     if (num_commands>0){
                        close(old_fds[0]);
                        close(old_fds[1]);
                     }
                     if (commands_rem>1){
                        old_fds[0] = new_fds[0];
                        old_fds[1] = new_fds[1];
                     }
                     waitpid(-1, NULL, WUNTRACED);
                     memset(curr_cmd, 0, sizeof(curr_cmd));
                     strcpy(curr_cmd, queues[i].pop(&queues[i]));
                     num_commands++;
                     commands_rem--;
                }            
            }
        }
    }
        return;
    }   

/* 
 * parseline - Parse the command line and build the argv array.
 */
int parseline(const char *cmdline, char **argv) 
{
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
	buf++;
	delim = strchr(buf, '\'');
    }
    else {
	delim = strchr(buf, ' ');
    }

    while (delim) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* ignore spaces */
	       buf++;

	if (*buf == '\'') {
	    buf++;
	    delim = strchr(buf, '\'');
	}
	else {
	    delim = strchr(buf, ' ');
	}
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* ignore blank line */
	return 1;

    return 1;
}


/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */
int builtin_cmd(char **argv) {
    if (strcmp(argv[0],"exit")==0){
        store_var(tsh_list);
        exit(0);
    } else if (strcmp(argv[0],"cd")==0){
        do_cd(argv);
        return 1;
    } else if (strcmp(argv[0],"int")==0){
        var_handle(tsh_list, argv);
        return 1;
    } else if (strcmp(argv[0],"cal")==0){
        cal_handle(tsh_list, argv);
        return 1;
    } else if (strcmp(argv[0],"display")==0){
        display_var(tsh_list, argv);
        return 1;
    }
    
    return 0;   /* not a builtin command */
}


/*****************
 * Signal handlers
 *****************/

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  
 */
void sigint_handler(int sig) 
{
    char c[100];
    printf("\nctrl+c Are you sure you want to exit? (Y/N) :");
    gets(c);
    if (c[0]=='Y'){
        exit(0);
    }
	printf("\n%s", prompt);
	fflush(stdout);
    return;
}


/*********************
 * End signal handlers
 *********************/


/***********************
 * Other helper routines
 ***********************/

/*
 * This function implements built-in command 'cd'
 */
void do_cd(char **argv)
{
	if(!chdir(argv[1]))
	{
		printf("Changed directory to %s \n", argv[1]);
	}
	else
	{
		printf("Error while setting home directory to %s errno:%d\n", argv[1], errno);
	}

}
