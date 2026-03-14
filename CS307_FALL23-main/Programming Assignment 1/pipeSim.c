#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){ 
    // shell process hierarchy
    int fd[2];
    int pipe_fd = pipe(fd); // fd[0] will read for pipe while fd[1] will be the writing end
    printf("I'm SHELL process, with PID: %d - Main command is: man ls | grep \"-R\" -a -w -m 1 -A 1\n", (int) getpid());
    // forking the MAN process 
    int man_fork = fork();
    if (man_fork < 0){ // if creating child process fails
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    // MAN process fork
    else if (man_fork == 0){ 
        printf("I'm MAN process, with PID %d - My command is: man ls\n", (int) getpid());
        char *man_args[3]; // creating arguments for execvp man to work
        man_args[0] = "man";
        man_args[1] = "ls";
        man_args[2] = NULL;
        close(fd[0]); // output pipe will not be used anymore - close it
        dup2(fd[1], STDOUT_FILENO); // duplicating fd[1] write - man command to pipe
        execvp(man_args[0], man_args); // execvp man

    }
    else{ // continue main process
        // forking the GREP process
        int grep_fork = fork(); 
        if (grep_fork < 0){ // if creating child process fails
            fprintf(stderr, "fork failed\n");
            exit(1);
        }
        // GREP process fork
        else if (grep_fork == 0){
            printf("I'm GREP process, with PID: %d - My command is grep \"-R\" -a -w -A 1 -m 1 \n", (int) getpid());
            close(fd[1]); // intput pipe will not be used anymore - close it
            dup2(fd[0], STDIN_FILENO); // duplicating fd[0] read - grep command to pipe
            close(STDOUT_FILENO);
            open("./output.txt",O_CREAT | O_WRONLY | O_TRUNC , S_IRWXU);

            char *grep_args[8];
            grep_args[0] = "grep";
            grep_args[1] = "-a";
            grep_args[2] = "-w";
            grep_args[3] = "-A 1";
            grep_args[4] = "-m 1";
            grep_args[5] = "--";
            grep_args[6] = "-R";
            grep_args[7] = NULL;
            execvp(grep_args[0], grep_args); // execvp grep
        }
        else{ // continue main process (from man)
            wait(NULL);
            printf("I'm SHELL process, with PID: %d - execution is completed, you can find the results in output.txt \n", (int) getpid());
        }
    }
    return 0;
}
