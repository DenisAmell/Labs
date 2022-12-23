#include <stdio.h>
#include <math.h>

const char *ErrorNames[] = {
    "Not opening file",
    "Out of Memory",
    "Not this index",
    "Wrong input flag",
    "Few arguments!",
    "Too many arguments!"};

enum Errors
{
    OPENING_FILE = 1,
    OUT_OF_MEMORY,
    NOT_INDEX,
    WRONG_INPUT,
    FEW_ARGUMENTS,
    MANY_ARGUMENTS
};

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

double my_atof(char *arr)
{
    double val = 0;
    int afterdot = 0;
    double scale = 1;
    int neg = 0;

    if (*arr == '-')
    {
        arr++;
        neg = 1;
    }
    while (*arr)
    {
        if (afterdot)
        {
            scale = scale / 10;
            val = val + (*arr - '0') * scale;
        }
        else
        {
            if (*arr == '.')
            {
                afterdot++;
            }
            else
            {
                val = val * 10.0 + (*arr - '0');
            }
        }
        arr++;
    }
    if (neg)
        return -val;
    else
        return val;
}

int func_q(double a, double b, double c, double *res, double *res2)
{
    double EPS = 0.0001;
    double D = b * b - 4.0 * a * c;
    if (D < -EPS)
    {
        return 0;
    }
    else if (fabs(D) < EPS)
    {
        *res = -b / (2.0 * a);
        return 1;
    }
    else
    {
        *res = (-b - sqrt(D)) / (2.0 * a);
        *res2 = (-b + sqrt(D)) / (2.0 * a);
        return 2;
    }

    return 0;
}

void func_m(int n, int m)
{
    if (m % n == 0)
    {
        printf("Yes");
    }
    else
    {
        printf("No");
    }
}

void func_t(int num1, int num2, int num3)
{
    int tmp;
    if (num1 > num2)
    {
        tmp = num1;
        num1 = num2;
        num2 = tmp;
    }
    if (num2 > num3)
    {
        tmp = num2;
        num2 = num3;
        num3 = tmp;
    }
    if (num1 * num1 + num2 * num2 == num3 * num3)
    {
        printf("Yes it is triangle");
    }
    else
    {
        printf("No");
    }
}

int main(int argc, char *argv[])
{
    double x1, x2;
    if (my_strlen(argv[1]) != 2 || argv[1][0] != '-' && argv[1][0] != '/')
    {
        printf(ErrorNames[WRONG_INPUT - 1]);
        return 0;
    }
    if (argc < 3)
    {
        printf(ErrorNames[FEW_ARGUMENTS - 1]);
        return 0;
    }
    if (argv[1][1] == 'q')
    {
        if (argc != 5)
        {
            printf(ErrorNames[FEW_ARGUMENTS - 1]);
            return 0;
        }
        // switch (func_q(my_atof(argv[2]), my_atof(argv[3]), my_atof(argv[4]), &x1, &x2))
        // {
        // case 0:
        //     printf("No roots");
        //     break;
        // case 1:

        //     break;
        // }
    }
    else if (argv[1][1] == 'm')
    {
        if (argc != 4)
        {
            printf(ErrorNames[FEW_ARGUMENTS - 1]);
            return 0;
        }
        func_m(my_atoi(argv[2]), my_atoi(argv[3]));
    }
    else if (argv[1][1] == 't')
    {
        if (argc != 5)
        {
            printf(ErrorNames[FEW_ARGUMENTS - 1]);
            return 0;
        }
        func_t(my_atoi(argv[2]), my_atoi(argv[3]), my_atoi(argv[4]));
    }
    else
    {
        printf(ErrorNames[WRONG_INPUT - 1]);
    }

    return 0;
}
