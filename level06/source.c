#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>

int     auth(char *s, unsigned int serial)
{
    int len;
    int n;

    s[strcspn(s, "\n")] = 0;
    len = strnlen(s, 32);
    if (len <= 5)
        return 1;
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
    {
        puts("\x1B[32m.---------------------------.");
        puts("\x1B[31m| !! TAMPERING DETECTED !!  |");
        puts("\x1B[32m'---------------------------'");
        return 1;
    }
    n = (((int)s[3]) ^ 0x1337) + 0x5eeded;
    for (int i = 0; i < len; i++)
    {
        if (s[i] <= 31)
            return 1;
        n += ((int)str[i] ^ key) % 1337;
    }
    return serial != n;
}

int     main(void)
{
    char s[32];
    unsigned int serial;

    puts("***********************************");
    puts("*\t\tlevel06\t\t  *");
    puts("***********************************");
    printf("-> Enter Login: ");
    fgets(s, 32, stdin);

    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");
    printf("-> Enter Serial: ");
    scanf("%u", &serial);

    if (!auth(s, serial))
    {
        puts("Authenticated!");
        system("/bin/sh");
        return 0;
    }
    return 1;
}