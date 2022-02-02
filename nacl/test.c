#include <stdio.h>
#include <string.h>
#include <sodium.h>

void hexdump(unsigned char *c, int size) {
    for (int i=0; i < size; i++) {
        printf("%02x ", c[i]);
    }
    printf("\n");
}

int bytewise_xor(unsigned char *a, unsigned char *b, unsigned char *out, int size) {

    for (int i=0; i < size; i++) {
        out[i] = a[i] ^ b[i];
    }

    return 1;
}

int main(int argc, char **argv)
{

    if (sodium_init() < 0) {
        /* panic! the library couldn't be initialized, it is not safe to use */
    }

    unsigned char a_sk[crypto_box_PUBLICKEYBYTES]; // n

    unsigned char a_pk[crypto_box_SECRETKEYBYTES]; // e G

    unsigned char b_sk[crypto_box_SECRETKEYBYTES]; // n

    unsigned char b_pk[crypto_box_SECRETKEYBYTES]; // e G

    unsigned char m[crypto_box_SECRETKEYBYTES] = "Hello";

    unsigned char cipher[crypto_box_SECRETKEYBYTES];

    unsigned char clear[crypto_box_SECRETKEYBYTES];

    unsigned char a_secret[crypto_box_SECRETKEYBYTES];

    unsigned char b_secret[crypto_box_SECRETKEYBYTES];

    crypto_box_keypair(a_pk, a_sk);

    crypto_box_keypair(b_pk, b_sk);
    
    // what a does

    // calc secret
    crypto_scalarmult(a_secret, a_sk, b_pk);
    printf("a_secret:");
    hexdump(a_secret, sizeof(a_secret));

    // enrcypt
    
    bytewise_xor(a_secret, m, cipher, 32);

    // what b does

    // calc sectret
    crypto_scalarmult(b_secret, b_sk, a_pk);
    printf("b_secret:");
    hexdump(b_secret, sizeof(b_secret));

    // decrypt

    bytewise_xor(b_secret, cipher, clear, 32);


    printf("%s\n", clear);

    crypto_stream()


    return 1;

}
