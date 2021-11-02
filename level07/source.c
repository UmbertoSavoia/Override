#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void    clear_stdin(void)
{
    unsigned char c = 0;

    do {
        c = getchar();
    } while (c != '\n' && c != -1);
}

unsigned int get_unum(void)
{
    unsigned int ret = 0;

    fflush(stdout);
    scanf("%u", &ret);
    clear_stdin();
    return ret;
}

int     read_number(int *buf)
{
    unsigned int n = 0;

    printf(" Index: ");
    n = get_unum();
    printf(" Number at data[%u] is %u\n", n, buf[n]);
    return 0;
}

int     store_number(int *buf)
{
    unsigned int n = 0;
    unsigned int i = 0;


    printf(" Number: ");
    n = get_unum();
    printf(" Index: ");
    i = get_unum();
    if ((i % 3) && ((n >> 0x18) != 0xb7))
    {
        buf[i] = n;
        return 0;
    }
    puts(" *** ERROR! ***");
    puts("   This index is reserved for wil!");
    puts(" *** ERROR! ***");
    return 1;
}

int     main(int argc, char **argv, char **envp)
{
    char **pargv = argv;
    char **penvp = envp;
    int  buf[100] = {0};
    char cmd[20];
    int  n = 0;

    while (*(*pargv))
        memset(*pargv, 0, strlen((*pargv)++));
    while (*(*penvp))
        memset(*penvp, 0, strlen((*penvp)++));
    puts("----------------------------------------------------\n"
         "  Welcome to wil's crappy number storage service!   \n"
         "----------------------------------------------------\n"
         " Commands:                                          \n"
         "    store - store a number into the data storage    \n"
         "    read  - read a number from the data storage     \n"
         "    quit  - exit the program                        \n"
         "----------------------------------------------------\n"
         "   wil has reserved some storage :>                 \n"
         "----------------------------------------------------\n");

    while (1)
    {
        printf("Input command: ");
        fgets(cmd, 20, stdin);
        cmd[strlen(cmd)-1] = 0;
        if (!strncmp(cmd, "store", 5))
            n = store_number(buf);
        else if (!strncmp(cmd, "read", 4))
            n = read_number(buf);
        else if (!strncmp(cmd, "quit", 4))
            return 0;

        if (!n)
            printf(" Completed %s command successfully\n", cmd);
        else
            printf(" Failed to do %s command\n", cmd);
    }
}