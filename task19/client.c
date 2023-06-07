#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    char filepath[256];
    printf("Enter the file path: ");
    fgets(filepath, sizeof(filepath), stdin);
    filepath[strcspn(filepath, "\n")] = '\0';

    /*call program name 'print'*/
    execlp("./print", "print", filepath, NULL);

    /*failt to execute execlp func*/
    perror("Error executing file output program");
    return 1;
}

