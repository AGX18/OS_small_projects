#include <stdio.h>

void foo(int d) {
    int arr[1000000];  // Allocate a large array to cause stack overflow
    printf("frame: %d\n", d);  
    foo(d + 1);  // Recursive call to itself
    // The function will keep calling itself until the stack overflows
    // and causes a segmentation fault or stack overflow error.
}

int main(void) {
    // Call the function to trigger stack overflow
    foo(0);

    // This line will not be reached if a stack overflow occurs
    printf("Stack overflow did not occur.\n");

    return 0;
}