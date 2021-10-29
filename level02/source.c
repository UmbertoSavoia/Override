#include <stdio.h>
#include <stdlib.h>

int     main(void)
{
    FILE *file;
    char stream[41];
    char s2[100];
    char s1[100];
    size_t tot;

    if ( (file = fopen("/home/users/level03/.pass", "r")) == 0 )
    {
        fwrite("ERROR: failed to open password file\n", 1, 0x24, stderr);
        exit(1);
    }
    tot = fread(stream, 1, 41, file);
    stream[strcspn(stream, "\n")] = 0;
    if (tot != 41)
    {
        fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
        fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
        exit(1);
    }
    fclose(file);
    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\**************************************/");
    printf("--[ Username: ");

    fgets(s2, 100, stdin);
    s2[strcspn(s2, "\n")] = 0;

    printf("--[ Password: ");
    fgets(s1, 100, stdin);
    s1[strcspn(s1, "\n")] = 0;

    puts("*****************************************");
    if (strncmp(stream, s2, 41) != 0)
    {
        printf(s2);
        puts(" does not have access!");
        exit(1);
    }
    printf("Greetings, %s!\n", s2);
    system("/bin/sh");
    return 0;
}