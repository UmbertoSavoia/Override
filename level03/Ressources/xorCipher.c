#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define fn(x) \
        do {  \
            printf(x); \
            exit(0); \
        }while (0)

int     main(int ac, char **av)
{
    if (ac != 3)
        fn("Usage:\n\t./xorCipher [string] [max key value]\n");

    char *str = av[1];
    int key = atoi(av[2]);
    int len = strlen(str);
    char ret[len];
    int i = 0;

    while (key--)
    {
        memset(ret, 0, len);
        puts("*************************");
        for (i = 0; str[i]; i++)
            ret[i] = str[i] ^ key;
        ret[i] = 0;
        printf("Key:      %d\n", key);
        printf("Stringa:  %s\n", ret);
        puts("*************************");
    }
}