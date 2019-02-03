#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    unsigned int left, algorithm;
    char buf[32];
    int fd;
    unsigned short random;
    struct timespec time_sp;
    
    if (argc != 2)
    {
        printf("Invalid argument\n");
    }

    algorithm = (argv[1][0] + argv[1][1] + argv[1][3] + argv[1][4] - 0x30 *4) % 5;
    
    left = (argv[1][0] -0x30) * 1000;
    left += (argv[1][1] -0x30) * 100;
   left += (argv[1][3] -0x30) * 10;
   left += (argv[1][4] -0x30);
   left = 10000 - left;
   
    switch (algorithm)
    {
    case 0:
        srand((unsigned) time(0));
        left = rand() % left;
        printf("%d%d.%d%d  algorithm = %d\n", left/1000, left%1000/100, left%100/10, left%10, algorithm);
        break;
    case 1:
        sprintf(buf, "echo $(($RANDOM%%%d))", left);
        system(buf);
        break;
    case 2:
        clock_gettime(CLOCK_REALTIME, &time_sp);
        left = time_sp.tv_nsec % left;
        printf("%d%d.%d%d  algorithm = %d\n", left/1000, left%1000/100, left%100/10, left%10, algorithm);
        break;
    case 3:
        fd = open("/dev/random", O_RDONLY);
        read(fd, &random, sizeof(random));
        left = random % left;
        printf("%d%d.%d%d  algorithm = %d\n", left/1000, left%1000/100, left%100/10, left%10, algorithm);
        break;
    case 4:
        fd = open("/proc/sys/kernel/random/uuid", O_RDONLY);
        read(fd, &random, sizeof(random));
        left = random % left;
        printf("%d%d.%d%d  algorithm = %d\n", left/1000, left%1000/100, left%100/10, left%10, algorithm);
        break;
    }
   return 0;
}

