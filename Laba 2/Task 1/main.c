#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int output_arr(int* arr, int size) {
    for(int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

}


int main(int argc, char* argv[]) {



    int size_A = 0;
    int size_B = 0;

    printf("Enter the size of the 1 array: ");
    scanf("%d", &size_A);
    printf("Enter the size of the 2 array: ");
    scanf("%d", &size_B);

    int *a = (int*)malloc(sizeof(int) * size_A);
    int *b = (int*)malloc(sizeof(int) * size_B);
    int *c = (int*)malloc(sizeof(int) * size_A);

    if (size_A <= 0 || size_B < 0) {
        printf("Incorrect input!\n");
        return 0;
    }

    srand(time(0));
    for(int i = 0; i < size_A; i++) {
        a[i] = rand() % 100 + 1;
        printf("%d ", a[i]);
    }
    printf("\n");
    for(int i = 0; i < size_B; i++) {
        b[i] = rand() % 100 + 1;
        printf("%d ", b[i]);
    }
    printf("\n");

    if (size_B <= 0) {
        output_arr(a, size_A);
    } else {
        for(int i = 0; i < size_A; i++) {
            if (i >= size_B) {
                c[i] = a[i] + b[i - 1];
            } else {
                c[i] = a[i] + b[i];
            }
        }
        output_arr(c,size_A);
    }

    free(a);
    free(b);
    free(c);


    return 0;
}