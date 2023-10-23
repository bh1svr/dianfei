#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
#include <ntstatus.h>
#define WIN32_NO_STATUS
#include <windows.h>
#include <bcrypt.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

int main(int argc, char *argv[])
{
    _Bool get_random = false;
#ifdef _WIN32
    unsigned long long random;
    if (BCryptGenRandom(NULL, (PUCHAR)&random, sizeof(random), BCRYPT_USE_SYSTEM_PREFERRED_RNG) == STATUS_SUCCESS)
    {
        get_random = true;
    }
#else
    unsigned long random;
    int fd = open("/dev/random", O_RDONLY);
    if (fd >= 0)
    {
        if (read(fd, &random, sizeof(random)) == sizeof(random))
        {
            get_random = true;
        }
        else
        {
            printf("Read /dev/random fail\n");
        }
        close(fd);
    }
    else
    {
        printf("Open /dev/random fail\n");
    }
#endif
    if (get_random)
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
        
    return 0;
}

