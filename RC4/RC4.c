/*
    Command line Input:
        a.exe "key" "hello world"
    Output:
        Encrypted text : 630958814BAF0C253A276C
        Dencrypted text : hello world
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 256 // 2^8

void swap(unsigned char *a, unsigned char *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int KSA(char *key, unsigned char *S)
{

    int len = strlen(key);
    int j = 0;

    for (int i = 0; i < N; i++)
        S[i] = i;

    for (int i = 0; i < N; i++)
    {
        j = (j + S[i] + key[i % len]) % N;

        swap(&S[i], &S[j]);
    }

    return 0;
}

int PRGA(unsigned char *S, char *plaintext, unsigned char *ciphertext)
{

    int i = 0;
    int j = 0;

    // size_t is unsigned int
    for (size_t n = 0, len = strlen(plaintext); n < len; n++)
    {
        i = (i + 1) % N;
        j = (j + S[i]) % N;

        swap(&S[i], &S[j]);
        int rnd = S[(S[i] + S[j]) % N];

        ciphertext[n] = rnd ^ plaintext[n];
    }

    return 0;
}

int RC4(char *key, char *plaintext, unsigned char *ciphertext)
{

    unsigned char S[N];
    KSA(key, S);

    PRGA(S, plaintext, ciphertext);

    return 0;
}

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        printf("Usage: %s <key> <plaintext>", argv[0]);
        return -1;
    }

    unsigned char *ciphertext = malloc(sizeof(int) * strlen(argv[2]));

    // Encryption
    RC4(argv[1], argv[2], ciphertext);
    printf("\nEncrypted text : ");
    for (size_t i = 0, len = strlen(argv[2]); i < len; i++)
        printf("%02hhX", ciphertext[i]);
    printf("\n");

    // Decryption
    unsigned char *plaintext = malloc(sizeof(int) * strlen(ciphertext));
    RC4(argv[1], ciphertext, plaintext);
    printf("Dencrypted text : ");
    for (size_t i = 0, len = strlen(ciphertext); i < len; i++)
        printf("%c", plaintext[i]);
    printf("\n")

        return 0;
}