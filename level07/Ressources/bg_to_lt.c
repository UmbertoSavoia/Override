#include <stdio.h>
#include <string.h>

int main(void)
{
    char s[] = "31c050682f2f7368682f62696e89e3505389e131d2b00b83e4f0cd80";
    int len = strlen(s);
    char *p = 0;
    char ctemp = 0;
    int itemp = 0;

    for (int i = 0; i < len; i+=8)
    {
        p = s + i;
        for (int j = 0, itemp = 6; j < 4; ++j)
        {
            if (j > 1)
                itemp = 2;
            ctemp = p[j];
            p[j] = p[j + itemp];
            p[j + itemp] = ctemp;
        }
    }
    for (int i = 0; i < len; ++i)
    {
        if ( i != 1 && i % 8 == 0 )
        {
            puts("");
            printf("0x");
        }
        printf("%c", s[i]);
    }
    puts("");
}