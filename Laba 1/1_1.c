#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

int my_atoi(char s[])
{
    int n = 0;
    for (int i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
    {

        n = 10 * n + (s[i] - '0');
    }
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

void func_h(long int num)
{
    int a = 0;
    // for (int i = 1; i < 101; i++)
    // {
    //     if (i % num == 0)
    //     {
    //         printf("%d ", i);
    //         a++;
    //     }
    // }
    int tmp = num;
    while (tmp <= 100)
    {
        printf("%d ", tmp);
        tmp += num;
        a++;
    }
    if (!a)
    {
        printf("No such numbers\n");
    }
}

int func_p(long int num)
{
    if (num > 1)
    {
        if (num == 2)
        {
            printf("prime");
            return 0;
        }
        if (num & 1 == 0)
        {
            printf("Not prime");
            return 0;
        }
        for (int i = 2; i <= floor(sqrt(num)); i++)
        {
            if (num % i == 0)
            {
                printf("Not prime");
                return 0;
            }
        }
        printf("Prime");
        return 1;
    }
    else
    {
        printf("Not prime");
        return 0;
    }
}

void func_s(char *num)
{
    int len = my_strlen(num);
    for (int i = 0; i < len; i++)
    {
        printf("%c ", num[i]);
    }
}

void func_e(long int num)
{
    if (num > 10)
    {
        printf("Input number must be <= 10");
    }
    else
    {
        for (int i = 1; i <= 10; i++)
        {
            printf("for %d: ", i);
            long long res = 1;
            for (int j = 1; j <= num; j++)
            {
                res *= i;
                printf("%lld ", res);
            }
            printf("\n");
        }
    }
}

void func_a(int num)
{
    int res = 0;
    for (int i = 1; i <= num; i++)
    {
        if (INT_MAX - i < res)
        {
            printf("Overflow error!\n");
            break;
        }
        else
        {
            res += i;
        }
    }
    if (res)
    {
        printf("%d", res);
    }
}

void func_f(long int num)
{
    unsigned long long res = 1;
    for (int i = 2; i <= num; i++)
    {
        if (ULLONG_MAX / i < res)
        {
            printf("Overflow error!\n");
            res = 0;
            break;
        }
        else
        {
            res *= i;
        }
    }
    if (res)
    {
        printf("%d", res);
    }
}

int main(int argc, char *argv[])
{
    if (my_strlen(argv[2]) != 2 || argv[2][0] != '-' && argv[2][0] != '/')
    {
        printf("Input error: wrong flag");
        return 0;
    }
    if (argc < 3)
    {
        printf("Input error: few arguments!");
        return 0;
    }
    else
    {
        if (argc == 3 && argv[2][1] == 'c')
        {
            printf("Input error: too few arguments for that flag!");
            return 0;
        }
        if (argc > 3)
        {
            printf("Input error: too many arguments!");
            return 0;
        }
    }
    if (argv[2][1] == 'h')
    {
        func_h(my_atoi(argv[1]));
    }
    else if (argv[2][1] == 'p')
    {
        func_p(my_atoi(argv[1]));
    }
    else if (argv[2][1] == 's')
    {
        func_s(argv[1]);
    }
    else if (argv[2][1] == 'e')
    {
        func_e(my_atoi(argv[1]));
    }
    else if (argv[2][1] == 'a')
    {
        func_a(my_atoi(argv[1]));
    }
    else if (argv[2][1] == 'f')
    {
        func_f(my_atoi(argv[1]));
    }
    else
    {
        printf("Input error: wrong flag!");
    }
    return 0;
}
