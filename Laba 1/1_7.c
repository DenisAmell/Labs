#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* create_arr(int* arr, int size, int a, int b) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % (b - a + 1) + a;
    }
    return arr;
}

void output_arr(int* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void swap(int *minPtr, int *maxPtr) {
    int tmp = *minPtr;
    *minPtr = *maxPtr;
    *maxPtr = tmp;
}

void find_min_max(int *arr, int size) {
    int min = arr[0]; int max = arr[0];
    int *max_A = arr;
    int *min_A = arr;
    for (int i = 0; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
            max_A = arr + i;
        } else if (arr[i] < min) {
            min = arr[i];
            min_A = arr + i;
        }
    }
    printf("Min elem: %d\n", min);
    printf("Max elem: %d\n", max);
    return swap(min_A, max_A);
}


int main(int argc, char* argv[]) {
    srand(time(0));
    int size, a, b;


    printf("Enter the size of the array: ");
    if(!scanf("%d", &size)){
        printf("Input error: wrong size");
        return 0;
    }
    if(size<=0){
        printf("Input error: wrong size");
        return 0;
    }

    printf("Range of numbers: ");
    scanf("%d%d", &a, &b);

    int* A = (int*)malloc(sizeof(int) * size);


    create_arr(A, size, a, b);
    printf("Array: ");
    output_arr(A, size);
    find_min_max(A, size);
    printf("Result: ");
    output_arr(A, size);

    free(A);
    return 0;
}