#include <stdio.h>
#include <stdlib.h>

void abnormal()
{
    int n = 1024;
    char *p = (char *)malloc(sizeof(char) * 1);

    free(p);
    free(p);                /* double free */
}

void abnormal_container()
{
    abnormal();
}

void normal()
{
    printf("normal function.\n");
}

int main(int argc, char **argv)
{
    abnormal_container();
    normal();
    return 0;
}
