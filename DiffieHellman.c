#include <stdio.h>

// function to find A = g^x mod n
int modulfun(int g, int x, int n)
{
    int r;
    int ans = 1;

    while (x > 0)
    {
        r = x % 2;

        if (r == 1)
        {
            ans = (ans * g) % n;
        }

        g = (g * g) % n;

        x = x / 2;
    }

    return ans;
}

int main()
{
    // Firstly Alice and Bob mutually agree on 2 large prime numbers, n & g.
    // These 2 numbers need not be kept secret.
    int n = 11, g = 7;
    // printf("Enter value of n and g\n");
    // scanf("%d %d", &n, &g);

    // Alice chooses another large random number x(private to her)
    printf("Enter the value of x\n");
    int x = 3;
    //scanf("%d", &x);
    // and calculates A such that A = g^x mod n.
    int A = modulfun(g, x, n);
    printf("A = g^x mod n =%d\n", A);
    // Alice sends this number A to Bob.

    // Bob independently chooses another large random number y(private to him)
    printf("Enter the value of y\n");
    int y = 6;
    //scanf("%d", &y);
    // and calculates B such that B = g^y mod n.
    int B = modulfun(g, y, n);
    printf("B = g^y mod n = %d\n", B);
    // Bob sends this number B to Alice.

    // Alice now computes her private key value K1 as: K1 = B^x mod n.
    int K1 = modulfun(B, x, n);

    // Bob computes his private key value K2 as: K2 = A^y mod n.
    int K2 = modulfun(A, y, n);

    printf("Alice's key K1 = B^x mod n = %d \nBob's key K2 = A^y mod n = %d\n", K1, K2);
}