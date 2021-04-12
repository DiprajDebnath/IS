#include <stdio.h>
 
// Function to compute a^m mod n
int compute(int a, int m, int n)
{
    int r;
    int y = 1;
 
    while (m > 0)
    {
        r = m % 2;
 
        // fast exponention
        if (r == 1)
            y = (y*a) % n;
        a = a*a % n;
 
        m = m / 2;
    }
 
    return y;
}

int main() {
    printf("%d",compute(5,6,23));
}