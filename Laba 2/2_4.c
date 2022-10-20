#include <stdio.h>
#include <math.h>
#include <stdarg.h>

double geom_mean(int count, ...) {
    double result = 1.0;
    va_list factor;
    va_start(factor, count);

    for(int i = 0; i < count; i++) {
        result *= va_arg(factor, double);
    }

    va_end(factor);

    if (count % 2 == 0 && result < 0.0) {
        return NAN;
    } else {
        return pow(result, 1.0/count);
    }
}

double pow_recurs(double num, int index)
{
    if (index == 0) {
        return 1;
    } else if (index == 1) {
        return num;
    } else if (index > 1) {
        if (index % 2 == 0) {
            return pow_recurs(num * num, index / 2);
        } else {
            return pow_recurs(num * num, index / 2) * num;
        }
    } else {
        return 1.0 / pow_recurs(num, index * (-1));
    }
}

int main(int argc, char *argv[]) {

    double result = geom_mean(4, (double) - 27.6, (double) - 1.9, (double) - 13.4, (double) - 2.0);
    if (isnan(result)) {
        printf("Error!");
    } else {
        printf("Average geometric: %f\n", result);
    }
    printf("Exponentiation: %f\n", pow_recurs(3, 4));
    return 0;
}