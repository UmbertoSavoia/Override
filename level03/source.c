#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void    decrypt(int key)
{
    char str[17] = "Q}|u`sfg~sf{}|a3";
    int len = strlen(str);
    for (int i = 0; i < len; ++i)
        str[i] ^= key;
    if (!strcmp(str, "Congratulations!"))
        system("/bin/sh");
    else
        puts("\nInvalid Password");
}

void    test(int input, int cons)
{
    int n = cons - input;

    switch (n) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            decrypt(n);
            break;
        default:
            n = rand();
            decrypt(n);
            break;
    }
}

int     main(void)
{
    int n;

    srand(time(0));
    puts("***********************************");
    puts("*\t\tlevel03\t\t**");
    puts("***********************************");
    printf("Password:");
    scanf("%d", &n);
    test(n, 322424845);
    return 0;
}