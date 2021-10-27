#include <string.h>
#include <stdio.h>

char buf[256];

int     verify_user_name()
{
    puts("verifying username....\n");
    return strncmp(buf, "dat_wil", 7);
}

int     verify_user_pass(char *s)
{
    return strncmp(s, "admin", 5);
}

int     main(void)
{
    char s[16];

    memset(s, 0, 16);
    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(buf, 256, stdin);
    if (verify_user_name() != 0)
    {
        puts("nope, incorrect username...\n");
        return 1;
    }
    puts("Enter Password: ");
    fgets(s, 100, stdin);
    if (verify_user_pass(s) != 0)
        return 0;

    puts("nope, incorrect password...\n");
    return 1;
}