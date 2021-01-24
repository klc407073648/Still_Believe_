#include <stdio.h>
#include <string.h>

int main(void)
{
    char str[] = "hello world itcast   myshell     string";
    //char str[] = "hello world itcast myshell string";

    char *p = NULL;
    char *src = str;

    while ((p = strsep(&src, " "))) {
        printf("%s\n", p);
    }

    return 0;
}
