#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_info
{
    char msg[140];
    char username[40];
    int size;
}               t_info;

void    secret_backdoor(void)
{
    char s[128];

    fgets(s, 128, stdin);
    system(s);
}

void    set_msg(t_info *user)
{
    char s[1024] = {0};

    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(s, 1024, stdin);
    strncpy(user->msg, s, user->size);
}

void    set_username(t_info *user)
{
    char s[128];

    puts(">: Enter your username");
    printf(">>: ");
    fgets(s, 128, stdin);
    for (int i = 0; i < 41 && s[i] != 0; ++i)
        user->username[i] = s[i];
    printf(">: Welcome, %s", user->username);
}

void    handle_msg(void)
{
    t_info user = {0};

    user.size = 140;

    set_username(&user);
    set_msg(&user);
    puts(">: Msg sent!");
}

int     main(void)
{
    puts("--------------------------------------------\n"
         "|   ~Welcome to l33t-m$n ~    v1337        |\n"
         "--------------------------------------------");
    handle_msg();
    return 0;
}