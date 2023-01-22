#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

enum ERRORS
{
    CONVEX = 1,
    NOT_CONVEX = -2,
    INPUT_ERROR = -1,
    NOT_MEMORY = -3,
    OK = 0,
};

typedef struct coord
{
    double x;
    double y;
} coord;

int convex(int count, ...)
{
    int x1, x2, y1, y2, res, tmp, tmp_sign;
    double EPS = 0.000001;
    int k = count / 2;
    coord *buff = (coord *)malloc(sizeof(coord) * k);
    if (buff == NULL)
    {
        return NOT_MEMORY;
    }

    if (count % 2 == 0 && count > 5)
    {
        va_list fact;
        va_start(fact, count);
        for (int i = 0; i < k; i++)
        {
            buff[i].x = va_arg(fact, double);
            buff[i].y = va_arg(fact, double);
        }
        va_end(fact);

        for (int i = 0; i < k; i++)
        {
            int ind_xy = i % k;
            int ind_xy1 = (i + 2) % k;
            int ind_xy2 = (i + 1) % k;

            x1 = buff[ind_xy2].x - buff[ind_xy].x;
            x2 = buff[ind_xy1].x - buff[ind_xy].x;

            y1 = buff[ind_xy2].y - buff[ind_xy].y;
            y2 = buff[ind_xy1].y - buff[ind_xy].y;

            res = (x1 * y2) - (y1 * x2);

            if (res > EPS)
                tmp = 1;
            else
                tmp = 0;

            if (i == 0)
            {
                tmp_sign = tmp;
            }
            else if (tmp_sign != tmp)
            {
                return NOT_CONVEX;
            }
        }
        return CONVEX;
    }
    return INPUT_ERROR;
}

int polynom(double *result, double x, int n, ...)
{
    va_list f;
    va_start(f, n);
    double coef;
    for (int i = 0; i <= n; i++)
    {
        coef = va_arg(f, double);
        *result += pow(x, i) * coef;
    }
    va_end(f);
    return OK;
}

int main()
{
    int status_code;
    double result = 0;
    status_code = convex(6, 1.0, 5.0, 2.0, 4.0, 7.0, 3.0);
    switch (status_code)
    {
    case CONVEX:
        printf("Is convex\n");
        break;
    case NOT_CONVEX:
        printf("Not convex\n");
        break;
    case INPUT_ERROR:
        printf("Input error\n");
        break;
    case NOT_MEMORY:
        printf("Out memory\n");
        break;
    }

    polynom(&result, 4.0, 3, 2.0, 1.0, 5.0, 4.0);
    printf("%lf", result);
    return 0;
}