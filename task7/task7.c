#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

void append_string(const char *file_path, const char *search_string, const char *new_string) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("can't open file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int found = 0;

    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        printf("can't create temp file\n");
        fclose(file);
        return;
    }
	/*search word and save in temp file
	*input new word to tempfile
	*/
    while (fgets(line, sizeof(line), file)) {
        fputs(line, temp_file);
        if (!found && strstr(line, search_string) != NULL) {
            found = 1;
            fputs(new_string, temp_file); 
        }
    }

    fclose(file);
    fclose(temp_file);

	/*copy temp file to original file*/
    remove(file_path);
    rename("temp.txt", file_path);

    if (found) {
        printf("search word success.\n");
    } else {
        printf("can't find word.\n");
    }
}

int main() {
    char search_string[MAX_LINE_LENGTH];
    char new_string[MAX_LINE_LENGTH];

    printf("input word for search: ");
    fgets(search_string, sizeof(search_string), stdin);
    search_string[strcspn(search_string, "\n")] = '\0';

    printf("input new word to add: ");
    fgets(new_string, sizeof(new_string), stdin);
    new_string[strcspn(new_string, "\n")] = '\0'; 

    append_string("example.txt", search_string, new_string);

    return 0;
}
