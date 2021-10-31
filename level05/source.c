#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int     main(void)
{
    char s[100];

    fgets(s, 100, stdin);

    for(int i = 0; i < strlen(s); i++)
        if (s[i] > 64 && s[i] <= 90)
            s[i] ^= 20;
    printf(s);
    exit(0);
}