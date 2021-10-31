#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <string.h>

int     main(void)
{
    char s[32];
    int status;
    int pid = fork();

    memset(s, 0, 32);
    if (pid == 0)
    {
        prctl(1, 1);
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        puts("Give me some shellcode, k");
        gets(s);
    }
    else if (pid > 0) {
        wait(&status);
        do {
            if (!(status & 0x7f))
            {
                puts("child is exiting...");
                return 0;
            }
        } while (ptrace(PTRACE_PEEKUSER, pid, 0x2c, 0) != 0x0b);
        puts("no exec() for you");
        kill(pid, 0x9);
    }
    return 0;
}