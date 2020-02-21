#include <windows.h>
#include <stdio.h>
#include <Wincrypt.h>
 
int main()
{
    short a = 0;
    HCRYPTPROV Rnd;
    LPCSTR UserName = "MyKeyContainer";
    if(CryptAcquireContext(&Rnd, UserName, NULL, PROV_RSA_FULL, 0))
    {
    } 
    else {
        if (GetLastError() == NTE_BAD_KEYSET) {
            CryptAcquireContext(
                &Rnd,
                UserName,
                NULL,
                PROV_RSA_FULL,
                CRYPT_NEWKEYSET);
        } else {
            printf("A cryptographic service handle could not be "
            "acquired.\n");
            exit(1);
        }
    }
 
    /*if (CryptGenRandom(Rnd, 4, (BYTE*)(&a)))*/
    if (CryptGenRandom(Rnd, sizeof(a), (BYTE*)(&a)))    
        printf("%.2f\n", a/32767.0*100);
    else
        puts("failed\n");
 
    CryptReleaseContext(Rnd,0); 
    return 0;
}