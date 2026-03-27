#include <stdio.h>

char *mystrcat(char *dst, const char *src)
{
    char *res = dst;
    while (*dst)
    {
        dst++;
    }
    while (*dst++ = *src++)
    {
    }
    return res;
}

int main(void)
{
    char s1[100] = "hello";
    char *s2 = "world";
    char *res = mystrcat(s1, s2);
    printf("%s", res);
    return 0;
}