#include <stdio.h>
#include <stdlib.h>


char* sc = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int sum(int num1, int num2) {
    int res = 0, carry = 0;
    res = num1 ^ num2;
    carry = (num1 & num2) << 1;
    while (carry) {
        int tmp = res;
        res = res ^ carry;
        carry = (tmp & carry) << 1;
    }
    return res;
}

int negtive(int n) {
    return sum(~n, 1);
}

int subtraction(int a, int b) {
    return sum(a, negtive(b));
}

int print(char* res, int SIZE) {
    for (int i = SIZE - 1; i >= 0; i--) {
        printf("%c", res[i]);
    }
}

int ten_into_n(unsigned int num, int r, char* res, int SIZE) {
    int count = 0;
    int temp = num;
    int k = 0;
    for (int i = 0; i <= SIZE; i++) {
        if (count >= SIZE) {
            SIZE <<= 1;
            res = (char*)realloc(res, SIZE * sizeof(char*));
        }
        if (num <= r) {
            if (count == 1) {
                break;
            }
            else {
                k = num;
                res[i] = sc[k];
                count = sum(count, 1);
                break;
            }
        }
        else {
            num >>= r;
            k = subtraction(temp, (num << r));
            res[i] = sc[k];
            temp >>= r;
            count = sum(count, 1);
        }
    }
    return print(res, count);
}



int main() {
    int SIZE = 4;
    int num, r;

    if (scanf("%d%d", &num, &r) != 2) {
        printf("Error");
        return -1;
    }
    char* result = (char*)malloc(sizeof(char) * SIZE);
    if (result == NULL) {
        printf("ban");
    }
    ten_into_n(num, r, result, SIZE);
    free(result);

    return 0;
}
