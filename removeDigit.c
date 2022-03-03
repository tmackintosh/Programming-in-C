#include <stdio.h>

void removeDigit(int x, int n, int* a_ptr, int* b_ptr){
    *a_ptr = 10;
    *b_ptr = 10;
}

int main() {
    int* a_ptr,* b_ptr;
    int a = 10;
    int b = 20;

    a_ptr = &a;
    b_ptr = &b;

    removeDigit(10, 10, a_ptr, b_ptr);
    return 0;
}