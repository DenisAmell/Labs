#include <stdio.h>
#include <math.h>
#include <stdlib.h>

char *sc = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int my_atoi(char s[])
{
    int n = 0, flag = 1;
    for (int i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
    {
        if (s[i] == '-')
        {
            flag = -1;
            continue;
        }
        n = 10 * n + (s[i] - '0');
    }
    n *= flag;
    return n;
}

int my_strlen(char s[])
{
    int len = 0;
    for (int i = 0; s[i] != '\0'; i++)
    {
        len++;
    }

    return len;
}

int my_strcmp(const char *s1, const char *s2)
{
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    while (*p1 && *p1 == *p2)
    {
        ++p1, ++p2;
    }

    return (*p1 > *p2) - (*p2 > *p1);
}

int print(char *res, int SIZE)
{
    for (int i = SIZE - 1; i >= 0; i--)
    {
        printf("%c", res[i]);
    }
}

void ten_into_n(unsigned int num, int r, char *res, size_t SIZE)
{
    int count = 0;
    int temp = num;
    int k = 0;
    for (int i = 0; i < SIZE; i++)
    {
        if (count >= SIZE)
        {
            SIZE *= 2;
            res = (char *)realloc(res, SIZE * sizeof(char *));
        } // 10
        if (num < r)
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
    print(res, count);
}

int to_dec(char *num, int r)
{
    int len = my_strlen(num) - 1;
    int res = 0, base = 1;
    int flag = 1;

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
        if (num[i] == '-')
        {
            flag = -1;
        }
    }
    res *= flag;
    return res;
}
int proverka(char num[100], int r)
{
    int len = my_strlen(num);
    for (int i = 0; i < len; i++)
    {
        if (num[i] >= sc[r])
        {
            return 0;
        }
    }
    return 1;
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

    printf("Enter base: ");
    scanf("%d", &r);
    if ((r < 2) || (r > 32))
    {
        printf("The number does not fall into the range [2...32]");
        return -1;
    }

    int num_max = 0;
    int num1;
    while (scanf("%100s", num))
    {
        if (my_strcmp(num, template) == 0)
        {
            break;
        }
        else if (my_atoi(num) == 0 && r <= 9)
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
        if (fabs(num1) > num_max)
        {
            num_max = fabs(num1);
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
