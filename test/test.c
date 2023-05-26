#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main() {
    char firstString[BUFFER_SIZE];
    char secondString[BUFFER_SIZE];

    // 파일 열기
    FILE* file = fopen("example.txt", "r+");
    if (file == NULL) {
        perror("파일 열기 실패");
        exit(1);
    }

    // 키보드로부터 문자열 입력
    printf("첫 번째 문자열 입력: ");
    scanf(" %[^\n]", firstString);

    printf("두 번째 문자열 입력: ");
    scanf(" %[^\n]", secondString);

    // 파일에서 첫 번째 문자열 찾기
    char buffer[BUFFER_SIZE];
    long currentPosition = 0;

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        char* foundPosition = strstr(buffer, firstString);
        if (foundPosition != NULL) {
            // 첫 번째 문자열을 찾았을 때 파일 포인터를 조정하여 두 번째 문자열을 추가
            long newPosition = currentPosition + (foundPosition - buffer) + strlen(firstString);

            if (fseek(file, newPosition, SEEK_SET) == -1) {
                perror("fseek 실패");
                exit(1);
            }

            if (fprintf(file, " %s", secondString) < 0) {
                perror("두 번째 문자열 쓰기 실패");
                exit(1);
            }

            // 작업 완료 후 프로그램 종료
            fclose(file);
            return 0;
        }

        currentPosition = ftell(file);
    }

    // 파일에서 첫 번째 문자열을 찾지 못한 경우
    printf("첫 번째 문자열을 찾을 수 없습니다.\n");

    // 파일 닫기
    fclose(file);
    return 0;
}

