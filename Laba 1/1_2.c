#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double func_q(double a, double b, double c)
{
    double EPS = 0.0001;
    double D = b * b - 4.0 * a * c;
    double x1, x2;
    if (D < 0)
    {
        printf("Еhere are no solutions");
        return 1;
    }
    else if ((fabs(D) < EPS))
    {
        x1 = -b / (2.0 * a);
        printf("%lf", x1);
        return 0;
    }
    else
    {
        x1 = (-b - sqrt(D)) / (2.0 * a);
        x2 = (-b + sqrt(D)) / (2.0 * a);
        printf("%lf%lf", x1, x2);
        return 0;
    }
}

int func_m(int n, int m)
{
    if (m % n == 0)
    {
        return printf("Yes");
    }
    else
    {
        printf("No");
    }
}

double func_t(double num1, double num2, double num3)
{
    if ((num1 == num2 + num3) || (num2 == num1 + num3) || (num3 == num1 + num2))
    {
        return printf("Yes it is triangle");
    }
    else
    {
        return printf("No");
    }
}

int main(int argc, char *argv[])
{

    if (strlen(argv[1]) != 2 || argv[1][0] != '-' && argv[1][0] != '/')
    {
        printf("Input error: wrong flag");
        return 0;
    }
    if (argc < 3)
    {
        printf("Input error: few arguments!");
        return 0;
    }
    if (argv[1][1] == 'q')
    {
        if (argc != 5)
        {
            printf("Input error: few arguments!");
            return 0;
        }
        func_q(strtod(argv[2], &argv[2]), strtod(argv[3], &argv[3]), strtod(argv[4], &argv[4]));
    }
    else if (argv[1][1] == 'm')
    {
        if (argc != 4)
        {
            printf("Input error: few arguments!");
            return 0;
        }
        func_m(atoi(argv[2]), atoi(argv[3]));
    }
    else if (argv[1][1] == 't')
    {
        if (argc != 5)
        {
            printf("Input error: few arguments!");
            return 0;
        }
        func_t(strtod(argv[2], &argv[2]), strtod(argv[3], &argv[3]), strtod(argv[4], &argv[4]));
    }
    else
    {
        printf("Input error: wrong flag!");
    }

    return 0;
}