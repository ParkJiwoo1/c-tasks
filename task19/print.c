#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_PATH_LENGTH 256

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Error creating pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Error forking process");
        return 1;
    }

    if (pid == 0) {
        /* Child process*/
        close(pipefd[1]);  /* Close the write end of the pipe*/

        /* Redirect stdin to read end of the pipe*/
        dup2(pipefd[0], STDIN_FILENO);

        /* Execute the file output program*/
        execlp("./client", "client", NULL);

        /* If execlp() returns, there was an error*/
        perror("Error executing file output program");
        exit(1);
    } else {
        /* Parent process*/
        close(pipefd[0]);  // Close the read end of the pipe

        char filepath[MAX_PATH_LENGTH];
        printf("Enter file path: ");
        fflush(stdout);

        /* Read file path from the user*/
        if (fgets(filepath, sizeof(filepath), stdin) == NULL) {
            perror("Error reading file path");
            return 1;
        }

        /* Write file path to the write end of the pipe*/
        if (write(pipefd[1], filepath, strlen(filepath)) == -1) {
            perror("Error writing to pipe");
            return 1;
        }

        close(pipefd[1]); 

        /* Wait for the child process to finish*/
        wait(NULL);

        printf("File output completed.\n");
    }

    return 0;
}

