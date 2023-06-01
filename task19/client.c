#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX_BUFFER_SIZE 256

int main() {
    char buffer[MAX_BUFFER_SIZE];

    /* Read the file path from stdin*/
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        perror("Error reading file path");
        exit(1);
    }

    /* Remove the newline character from the file path*/
    size_t length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    }

    /* Open the file*/
    FILE* file = fopen(buffer, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    /* Read and print the file contents*/
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);

    return 0;
}

