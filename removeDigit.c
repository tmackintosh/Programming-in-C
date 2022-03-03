#include <stdio.h>
#include <stdbool.h>

int contains(int value, int digit) {
    int order_of_magnitude = 0;

    while (value != 0) {
        int num = value % 10;

        if (num == digit) {
            return true;
        }

        value /= 10;
        order_of_magnitude += 1;
    }

    return false;
}

void removeDigit(int x, int n, int* a_ptr, int* b_ptr){
    int a, b;

    if (n % 2 == 0) {
        a = (n / 2) - 1;
        b = (n / 2) + 1;
    } else {
        a = (n / 2);
        b = (n / 2) + 1;
    }

    while (true) {
        if (!contains(a, x) && !contains(b, x)) {
            *a_ptr = a;
            *b_ptr = b;

            return;
        }

        a -= 1;
        b += 1;
    }
}

int main() {
    int* a_ptr,* b_ptr, a, b;

    a_ptr = &a;
    b_ptr = &b;

    removeDigit(3, 202, a_ptr, b_ptr);

    printf("A: %d\n", *a_ptr);
    printf("B: %d\n", *b_ptr);

    return 0;
}