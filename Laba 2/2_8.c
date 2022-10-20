#include <stdio.h>
#include <math.h>


double f(double x) {
    return 4 - exp(x) - 2 * pow(x, 2);
}

double met_dichotomy(double a, double b, double EPS) {
    double c = 0.0;

    while ( b - a > EPS) {
        c = (a + b) / 2;

        if (fabs(f(c)) < EPS) {
            return c;
        }else if (f(b) * f(c) < (-1) * EPS) {
            a = c;
        } else {
            b = c;
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

    printf("Result x = %f\n", result);

    return 0;
}
