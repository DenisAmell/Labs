#include <stdio.h>
#include <math.h>


double f(double x) {
    return x - 2 + sin(1/x);
}

double met_dichotomy(double a, double b, double EPS) {
    double c = 0.0;

    while ( b - a > EPS) {
        c = (a + b) / 2;

        if (fabs(f(c)) < EPS) {
            return c;
        }else if (f(b) * f(c) < EPS) {
            a = c;
        } else if (f(c) * f(a) < EPS) {
            b = c;
        } else {
            return NAN;
        }
    }

    return (a + b) / 2;
}

int main(int argc, char *argv[]) {
    double x_left, x_right, EPS;

    printf("Enter a: ");
    scanf("%lf", &x_left);
    printf("Enter b: ");
    scanf("%lf", &x_right);
    printf("Enter epsilon: ");
    scanf("%lf", &EPS);

    double result = met_dichotomy(x_left, x_right, EPS);

    if(!isnan(result)) {
        printf("Result x = %f\n", result);
    }
    else {
        printf("No solutions in given interval");
    }

    return 0;
}
