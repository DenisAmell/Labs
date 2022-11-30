#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

char *sc = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int print(char *res, int SIZE)
{
    for (int i = SIZE - 1; i >= 0; i--)
    {
        printf("%c", res[i]);
    }
}

int ten_into_n(unsigned int num, int r, char *res, size_t SIZE)
{
    int count = 0;
    int temp = num;
    int k = 0;
    for (int i = 0; i <= SIZE; i++)
    {
        if (count >= SIZE)
        {
            SIZE <<= 1;
            res = (char *)realloc(res, SIZE * sizeof(char *));
        }
        if (num <= r)
        {
            k = num;
            res[i] = sc[k];
            count++;
            break;
        }
        else
        {
            num /= r;
            k = temp - (num * r);
            res[i] = sc[k];
            temp /= r;
            count++;
        }
    }
    return print(res, count);
}

int to_dec(char *num, int r)
{
    int len = strlen(num) - 1;
    int res = 0, base = 1;
    for (int i = len; i >= 0; i--)
    {
        if (num[i] >= '0' && num[i] <= '9')
        {
            res += (num[i] - 48) * base;
            base *= r;
        }
        if (num[i] >= 'A' && num[i] <= 'Z')
        {
            res += (num[i] - 55) * base;
            base *= r;
        }
    }
    return res;
}

int proverka(char num[100], int r)
{
    int len = strlen(num);
    for (int i = 0; i < len; i++)
    {
        if (num[i] >= sc[r])
        {
            return 0;
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    int r;
    char num[100];
    char *template = "Stop";
    size_t size = 10;
    char *res = (char *)malloc(sizeof(char) * size);
    if (res == NULL)
    {
        printf("Error: not memory");
        return -1;
    }

    printf("Enter r: ");
    scanf("%d", &r);
    if ((r < 2) || (r > 32))
    {
        printf("The number does not fall into the range [2...36]");
        return -1;
    }

    int i = 0;
    int num_max = 0;
    int num1;
    while (scanf("%100s", num))
    {
        if (strcmp(num, template) == 0)
        {
            break;
        }
        else if (atoi(num) == 0 && r <= 9)
        {
            printf("No number\n");
            continue;
        }
        else if ((proverka(num, r)) == 0)
        {
            printf("No number base\n");
            continue;
        }

        num1 = to_dec(num, r);
        if (abs(num1) > num_max)
        {
            num_max = num1;
        }
    }

    printf("Max number: ");
    ten_into_n(num_max, r, res, size);

    int p = 1;
    while (p <= 4)
    {
        printf("\n");
        printf("Max number in the number system with a base %d: ", p * 9);
        ten_into_n(num_max, p * 9, res, size);
        p++;
    }

    free(res);
    return 0;
}
