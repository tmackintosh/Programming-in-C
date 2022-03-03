#include <stdio.h>
#include <stdbool.h>

// Functions determines whether a number contains a specific digit
// @param value int of the number to determine
// @param digit int of the number to assess against
// @returns bool of whether the value contains the digit
bool contains(int value, int digit) {
    // Traverse the whole number until we've assessed each digit
    while (value != 0) {
        // Get current units digit
        int num = value % 10;

        if (num == digit) {
            return true;
        }

        // Shift the value down by 1 digit
        value /= 10;
    }

    return false;
}

// See which 2 numbers sum to n, but neither includes x as a digit
// @param x int of the digit we don't like
// @param n int of the number to get the sum to
// @param a_ptr int* of the memory location to adjust with the first operator of the sum
// @param b_ptr int* of the memory location to adjust with the second operator of the sum
// @returns void
void removeDigit(int x, int n, int* a_ptr, int* b_ptr){
    int a, b;

    // Start from the midpoind and work outwards
    if (n % 2 == 0) {
        a = (n / 2) - 1;
        b = (n / 2) + 1;
    } else {
        a = (n / 2);
        b = (n / 2) + 1;
    }

    // Continue working outwards until we get a match
    // This loop returns from the function
    while (true) {
        if (!contains(a, x) && !contains(b, x)) {
            *a_ptr = a;
            *b_ptr = b;

            return;
        }

        // This could be improved by incrementing for the digit that contained
        // x, for example 321 contains a 3, so we increment by 100 to remove the 3
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