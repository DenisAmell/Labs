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

void change(int* minPtr, int* maxPtr) {
    int min = *minPtr;
    int max = *maxPtr;
    *minPtr = max;
    *maxPtr = min;
}

void replace(int* arr, int size) {
    int min = arr[0]; int max = arr[0];
    int* max_A;
    int* min_A;
    for (int i = 0; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
            max_A = arr + i;
        } else if (arr[i] < min) {
            min = arr[i];
            min_A = arr + i;
        }
    }
    return change(min_A, max_A);
}

int new_resArr(int* arr, int* res, int size) {
    int new_size = 1;
    res[0] = *arr;
    for (int i = 1; i < size; i++) {
        int flag = 0;
        for(int j = 0; j < size; j++) {
            if (arr[i] == res[j]) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
           res[new_size] = arr[i];
           new_size++;
        }
    }
    return new_size;
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



    printf("1. Swap the max and min elements\n");
    printf("2. Create a new array\n");
    printf("Enter: ");
    int input = 0;
    if(!scanf("%d", &input)){
        printf("Input error!");
        return 0;
    }
    if(input == 1){
        replace(A, size);
        output_arr(A, size);
    }
    else if(input == 2){
        int* res = (int*)malloc(size*sizeof(int));
        int size_res = new_resArr(A, res, size);
        output_arr(res, size_res);
        free(res);
    }
    else{
        printf("Input error!");
        return 0;
    }

    free(A);
    getchar();
    return 0;
}
