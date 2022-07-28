#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    unsigned long random;
    int fd = open("/dev/random", O_RDONLY);
    if (fd >= 0)
    {
        if (read(fd, &random, sizeof(random)) == sizeof(random))
        {
            if (argc == 2)
            {
                 unsigned long max = strtoul(argv[1], NULL, 10);
                 unsigned long result = random % max + 1;
                 printf("%lu\n", result);
            }
            else
            {
                unsigned int result = random % 10000 + 1;
                printf("%d.%d\n", result / 100, result % 100);
            }
        }
        else
        {
            printf("Read /dev/random fail\n");
        }
    }
    else
    {
        printf("Open /dev/random fail\n");
    }
    close(fd);
    return 0;
}

