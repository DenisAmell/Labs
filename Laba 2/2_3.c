#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double fact(double n)
{
    if (n == 0 || n == 1)
    {
        return 1;
    }
    else
    {
        return n * fact(n - 1);
    }
}

int isPrime(int n)
{
    for (int i = 2; i <= sqrt(n); i++)
    {
        if ((n % i) == 0)
            return 0;
    }
    return 1;
}

double lim_e(double eps)
{
    double n = 1;
    double an = pow(1 + 1 / n, n);
    double an_1 = pow(1 + 1 / (n + 1), n + 1);

    while (eps < fabs(an_1 - an))
    {
        n++;
        an = an_1;
        an_1 = pow(1 + 1 / (n + 1), n + 1);
    }
    return an;
}

double lim_pi(double eps)
{
    double n = 1;
    double an = pow((pow(2, n) * fact(n)), 4) / (n * pow(fact(2 * n), 2.0));
    double an_1 = pow((pow(2, n + 1) * fact(n + 1)), 4) / ((n + 1) * pow(fact(2 * (n + 1)), 2.0));
    while (eps < fabs(an_1 - an))
    {
        n++;
        an = an_1;
        an_1 = pow((pow(2, n + 1) * fact(n + 1)), 4) / ((n + 1) * pow(fact(2 * (n + 1)), 2));
    }
    return an;
}

double lim_ln(double eps)
{
    double n = 1;
    double an = n * (pow(2, 1 / n) - 1);
    double an_1 = (n + 1) * (pow(2, 1 / (n + 1)) - 1);

    while (eps < fabs(an_1 - an))
    {
        n++;
        an = an_1;
        an_1 = (n + 1) * (pow(2, 1 / (n + 1)) - 1);
    }
    return an;
}

double lim_sqr(double eps)
{
    double an = -0.5;
    double an_1 = an - pow(an, 2) / +1;

    while (eps < fabs(an_1 - an))
    {
        an = an_1;
        an_1 = an - pow(an, 2) / 2 + 1;
    }
    return an;
}

double summa(double m)
{
    double k = 1;
    double sum = 0;

    while (k <= m)
    {
        sum += (fact(m) / (fact(m - k) * fact(k))) * (pow(-1, k) / k) * log(fact(k));
        k++;
    }
    return sum;
}

double lim_gamma(double eps)
{
    double n = 1;
    double an = summa(n);
    double an_1 = summa(n + 1);

    while (eps < fabs(an_1 - an))
    {
        n++;
        an = an_1;
        an_1 = summa(n + 1);
    }

    return an;
}

double sum_e(double eps)
{
    double cur = 1;
    double sum = 0;
    double n = 0;

    while (fabs(cur) > eps)
    {
        cur = 1 / fact(n);
        sum += cur;
        n++;
    }
    return sum;
}

double sum_pi(double eps)
{
    double cur = 1;
    double sum = 0;
    double n = 1;

    while (fabs(cur) >= eps)
    {
        cur = pow(-1, n - 1) / (2 * n - 1);
        sum += cur;
        n++;
    }
    return 4 * sum;
}

double sum_ln(double eps)
{
    double cur = 1;
    double sum = 0;
    double n = 1;

    while (fabs(cur) >= eps)
    {
        cur = pow(-1, n - 1) / n;
        sum += cur;
        n++;
    }
    return sum;
}

double multy_sqr(double eps)
{
    double cur = 1;
    double mult = 1;
    double n = 2;

    while (n <= 1000)
    {
        cur = pow(2, pow(2, n * (-1)));
        mult *= cur;
        n++;
    }

    return mult;
}

double sum_gamma(double eps)
{
    double cur = 1;
    double sum = 0;
    double n = 2;

    while (fabs(cur) >= eps)
    {
        cur = 1 / pow(floorf(sqrt(n)), 2) - (1 / n);
        sum += cur;
        n++;
    }

    return (pow(sum_pi(eps), 2) / 6) - sum;
}

double f_e(double x)
{
    return log(x) - 1;
}

double f_pi(double x)
{
    return cos(x) + 1;
}

double df_pi(double x)
{
    return -sin(x);
}

double f_ln(double x)
{
    return exp(x) - 2;
}

double f_sqr(double x)
{
    return pow(x, 2) - 2;
}

double multy_gamma(double t)
{
    int p = 2;
    double mult = 1;

    while (p <= t)
    {
        if (isPrime(p))
        {
            mult *= (1.0 - 1.0 / p);
        }
        p++;
    }
    return mult;
}

double f_gamma(double x, double eps)
{
    double t = 0;
    double an = log(t) * multy_gamma(t);
    double an_1 = log(t + 1) * multy_gamma(t + 1);

    while (eps < fabs(an_1 - an))
    {
        t++;
        an = an_1;
        an_1 = log(t + 1) * multy_gamma(t + 1);
    }

    return exp(x * (-1)) - an;
}

double count_gamma(double eps)
{
    double l = -1;
    double r = 1, c;

    while (r - l > eps)
    {
        c = (l + r) / 2;
        if (f_gamma(c, eps) * f_gamma(r, eps) < 0)
            l = c;
        else
            r = c;
    }
    return (l + r) / 2;
}

// double symb_p(int t)
// {
//     double x = 1.0;
//     for (int i = 2; i <= t; i++)
//     {
//         if (isPrime)
//         {
//             x *= (1.0 - 1.0 / i);
//         }
//     }
//     return x;
// }

// double mult_gamma(double eps)
// {
//     double tmp1 = 0;
//     double tmp2 = 0;
//     for (int t = 2;; t++)
//     {
//         tmp1 = tmp2;
//         tmp2 = log(t) * symb_p(t);
//         if (fabs(tmp2 - tmp1) < eps)
//         {
//             break;
//         }
//     }
//     return tmp2;
// }

// double count_gamma(double x)
// {
//     return exp(-x) - mult_gamma(0.001);
// }

double count_e(double eps)
{
    double l = 0;
    double r = 3, c;

    while (r - l > eps)
    {
        c = (l + r) / 2;
        if (f_e(c) * f_e(r) < 0)
            l = c;
        else
            r = c;
    }
    return (l + r) / 2;
}

double count_pi(double eps)
{
    double a = 3, b = 4;
    double x = (a + b) / 2;
    double x1 = x + 1;

    while (fabs(x - x1) > eps)
    {
        x1 = x;
        x = x - f_pi(x) / df_pi(x);
    }
    return x;
}

double count_ln(double eps)
{
    double l = 0;
    double r = 1, c;

    while (r - l > eps)
    {
        c = (l + r) / 2;
        if (f_ln(c) * f_ln(l) > 0)
            l = c;
        else
            r = c;
    }
    return (l + r) / 2;
}

double count_sqr(double eps)
{
    double l = 0;
    double r = 2;
    double c;

    while (r - l > eps)
    {
        c = (l + r) / 2;
        if (f_sqr(c) * f_sqr(r) < 0)
            l = c;
        else
            r = c;
    }
    return (l + r) / 2;
}

int main()
{
    double eps = 0;
    printf("Enter accuracy (eps) : ");
    if (scanf("%lf", &eps) != 1)
    {
        printf("Input error");
        return -1;
    }
    printf("e = %.10lf\n", lim_e(eps));
    printf("pi = %.10lf\n", lim_pi(eps));
    printf("ln2 = %.10lf\n", lim_ln(eps));
    printf("sqr(2) = %.10lf\n", lim_sqr(eps));
    printf("gamma = %.10lf\n", lim_gamma(eps));
    printf("e = %.10lf\n", sum_e(eps));
    printf("pi = %.10lf\n", sum_pi(eps));
    printf("ln2 = %.10lf\n", sum_ln(eps));
    printf("sqr(2) = %.10lf\n", multy_sqr(eps));
    printf("gamma = %.10lf\n", sum_gamma(eps));
    printf("e = %.10lf\n", count_e(eps));
    printf("pi = %.10lf\n", count_pi(eps));
    printf("ln2 = %.10lf\n", count_ln(eps));
    printf("sqr(2) = %.10lf\n", count_sqr(eps));
    printf("gamma = %.10lf\n", count_gamma(eps));

    return 0;
}
