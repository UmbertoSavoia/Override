#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void    log_wrapper(FILE *file, char *s, char *arg)
{
    char buf[256];

    strcpy(buf, s);
    snprintf(buf, strlen(arg), arg);
    buf[strcspn(buf, "\n")] = 0;
    fprintf(file, "LOG: %s\n", buf);

}

int     main(int argc, char **argv)
{
    if (argc != 2)
        printf("Usage: %s filename\n", argv[0]);
    FILE *log = 0;
    FILE *stream = 0;
    char dest[104];
    int fd;
    char c;

    if (!(log = fopen("./backups/.log", "w")))
    {
        printf("ERROR: Failed to open %s\n", "./backups/.log");
        exit(1);
    }
    log_wrapper(log, "Starting back up: ", argv[1]);
    if (!(stream = fopen(argv[1], "r")))
    {
        printf("ERROR: Failed to open %s\n", argv[1]);
        exit(1);
    }
    strcpy(dest, "./backups/");
    strncat(dest, argv[1], strlen(argv[1]));
    if ((fd = open(dest, 0xc1, 0x1b0)) < 0)
    {
        printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
        exit(1);
    }
    while (c = fgetc(stream))
        write(fd, &c, 1);
    log_wrapper(log, "Finished back up ", argv[1]);

    fclose(stream);
    close(fd);
    return 0;
}