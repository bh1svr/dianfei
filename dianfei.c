#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    unsigned short random;
    
    int fd = open("/dev/random", O_RDONLY);
    read(fd, &random, sizeof(random));
    float left = random / 65535.0 * 100.0;
    printf("%.2f\n", left);
    return 0;
}

