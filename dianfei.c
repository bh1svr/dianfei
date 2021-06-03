#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    unsigned short random;
    
    int fd = open("/dev/random", O_RDONLY);
    if (fd >= 0)
    {
        if (read(fd, &random, sizeof(random)) == sizeof(random))
        {
            float left = random / 65535.0 * 100.0;
            printf("%.2f\n", left);
        }
        else
        {
            printf("Rean /dev/random fail\n");
        }
    }
    else
    {
        printf("Open /dev/random fail\n");
    }
    close(fd);
    return 0;
}

