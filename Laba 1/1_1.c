#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void func_h(int num) {
    int a = 0;
    for (int i = 0; i < 101; i++) {
        if (i % num == 0) {
            printf("%d ", i);
            a++;
        }
    }
    if (!a) {
        printf("No such numbers\n");
    }
}

void func_p(int num) {
    int d = 0;
    for (int i = 1; i <= num; i++) {
        if (num % i == 0) {
            d++;
        }
        if (d > 2) {
            printf("Input number is not prime");
            break;
        }
    }
    if (d == 2) {
        printf("Input number is prime");
    }
}

void func_s(int num) {
    int tmp = num, len = 0;
    while (tmp) {
        len++;
        tmp /= 10;
    }

    tmp = len;

    int res[len];
    while (num) {
        res[len - 1] = num % 10;
        len--;
        num /= 10;
    }
    for (int i = 0; i < tmp; i++) {
        printf("%d ", res[i]);
    }
}

void func_e(int num) {
    if(num>10) {
        printf("Input number must be <= 10");
    } else {
        for (int i = 1; i <= 10; i++) {
            printf("for %d: ", i);
            unsigned long res = 1;
            for (int j = 1; j <=num; j++) {
                res *= i;
                printf("%lu ", res);
            }
            printf("\n");
        }
    }
}

void func_a(int num){
    int res = 0;
    for (int i = 1; i <=num; i++) {
        if(INT_MAX-i<res)
        {
            printf("Overflow error!\n");
            res = 0;
            break;
        } else {
            res += i;
        }
    }
    if (res) {
        printf("%d", res);
    }

}

void func_f(int num) {
    unsigned long long res = 1;
    for (int i = 2; i <=num; i++) {
        if (ULLONG_MAX / i < res) {
            printf("Overflow error!\n");
            res = 0;
            break;
        } else {
            res *= i;
        }
    }
    if (res) {
        printf("%d", res);
    }

}

int main(int argc, char* argv[]){
    if ((argv[2][0] != '-') || (argv[2][0] != '/')) {
        printf("Input error: wrong flag");
        return 0;
    }
    if(strlen(argv[2])!=2){
        printf("Input error: wrong flag!");
        return 0;
    }
    if(argc < 3){
        printf("Input error: few arguments!");
        return 0;
    }
    else{
        if(argc == 3 && argv[2][1] == 'c'){
            printf("Input error: too few arguments for that flag!");
            return 0;
        }
        if(argc>4){
            printf("Input error: too many arguments!");
            return 0;
        }
    }
    if (argv[2][1] == 'h') {
        func_h(argv[1]);
    } else if (argv[2][1] == 'p') {
        func_p(argv[1]);
    } else if (argv[2][1] == 's') {
        func_s(argv[1]);
    } else if (argv[2][1] == 'e') {
        func_e(argv[1]);
    } else if (argv[2][1] == 'a') {
        func_a(argv[1]);
    } else if (argv[2][1] == 'f') {
        func_f(argv[1]);
    } else {
        printf("Input error: wrong flag!");
    }
    return 0;
}