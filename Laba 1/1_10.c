#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

const double EPS = 1E-9;

typedef enum Errors
{
    ok,
    memory_allocation_error,
    range_error,
    multi_error

} Errors;

void free_matrix(double **matrix, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

void print_matrix(double **res, int N, int M)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            printf("%lf ", res[i][j]);
        }
        printf("\n");
    }
}

// typedef struct dinamic_array
// {
//     int **matrix;
// } dinamic_array;

double **generate_matrix(Errors *status_code, int n, int m)
{

    double **matrix;

    if (n < 1 || n > 10 || m < 1 || m > 10)
    {
        *status_code = range_error;
        return matrix;
    }

    matrix = (double **)malloc(sizeof(double *) * n);
    for (int i = 0; i < m; i++)
    {
        matrix[i] = (double *)malloc(sizeof(double) * m);
        if (matrix[i] == NULL)
        {
            *status_code = memory_allocation_error;
            return matrix;
        }
    }
    if (matrix == NULL)
    {
        *status_code = memory_allocation_error;
        return matrix;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            matrix[i][j] = (double)rand() / (double)RAND_MAX * 200 - 100;
        }
    }
    *status_code = ok;
    return matrix;
}

double **multi_matrix(Errors *status_code, double **A, double **B, int n, int m, int row2, int column1)
{
    double **res;

    if (column1 != row2)
    {
        *status_code = multi_error;
        return res;
    }
    res = (double **)malloc(sizeof(double *) * n);
    if (res == NULL)
    {
        *status_code = memory_allocation_error;
        return res;
    }
    for (int i = 0; i < m; i++)
    {
        res[i] = (double *)malloc(sizeof(double) * m);
        if (res[i] == NULL)
        {
            *status_code = memory_allocation_error;
            free_matrix(res, n, i);
            return res;
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            res[i][j] = 0;
            for (int k = 0; k < n; k++)
            {
                res[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    *status_code = ok;
    return res;
}

void swap(double *num1, double *num2)
{
    double tmp;
    tmp = *num1;
    *num1 = *num2;
    *num2 = tmp;
}

double det(double **matrix, int n)
{
    double det = 1;
    if (n == 2)
    {
        det = matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
        return det;
    }
    for (int i = 0; i < n; ++i)
    {
        int k = i;
        for (int j = i + 1; j < n; ++j)
            if (fabs(matrix[j][i]) > fabs(matrix[k][i]))
                k = j;
        if (fabs(matrix[k][i]) < EPS)
        {
            det = 0;
            break;
        }
        swap(matrix[i], matrix[k]);
        if (i != k)
            det = -det;
        det *= matrix[i][i];
        for (int j = i + 1; j < n; ++j)
            matrix[i][j] /= matrix[i][i];
        for (int j = 0; j < n; ++j)
            if (j != i && fabs(matrix[j][i]) > EPS)
                for (int k = i + 1; k < n; ++k)
                    matrix[j][k] -= matrix[i][k] * matrix[j][i];
    }
    return det;
}
int main()
{

    srand(time(0));
    int row1 = 0, column1 = 0;
    int row2 = 0, column2 = 0;
    Errors status_code;

    printf("1. Ganarate matrix.\n");
    printf("2. Matrix multiplication.\n");
    printf("3. Find det matrix.\n");
    printf("Enter: ");
    int input = 0;
    if (!scanf("%d", &input))
    {
        printf("Input error!");
        return 0;
    }
    if (input == 1)
    {
        printf("Enter row and column: ");
        scanf("%d%d", &row1, &column1);

        double **A;
        A = generate_matrix(&status_code, row1, column1);
        switch (status_code)
        {
        case ok:
            print_matrix(A, row1, column1);
            free_matrix(A, row1, column1);
            break;
        case memory_allocation_error:
            printf("Error: not memory");
            break;
        case range_error:
            printf("Enter in the range [1..10]");
            break;
        }
    }
    else if ((input > 1) && (input <= 3))
    {
        printf("Enter row and column for matrix 1: ");
        scanf("%d%d", &row1, &column1);
        printf("Enter row and column for matrix 2: ");
        scanf("%d%d", &row2, &column2);
        if ((input == 3) && ((row1 != column1) && (row2 != column2)))
        {
            printf("Error: non-quadratic matrix\n");
            printf("It is not possible to calculate");
            return 0;
        }
        double **A = generate_matrix(&status_code, row1, column1);
        double **B = generate_matrix(&status_code, row2, column2);
        double **result = multi_matrix(&status_code, A, B, row1, column2, row2, column1);

        switch (status_code)
        {
        case ok:
            printf("Matrix 1: \n");
            print_matrix(A, row1, column1);
            printf("Matrix 2: \n");
            print_matrix(B, row2, column2);
            printf("Matrix multiplication: \n");
            print_matrix(result, row1, column2);
            if (input == 3)
            {

                printf("Det Matrix 1: %lf\n", det(A, row1));
                printf("Det Matrix 2: %lf\n", det(B, row2));
                printf("Det multiplication: %lf\n", det(result, row1));
            }
            free_matrix(A, row1, column1);
            free_matrix(B, row2, column2);
            free_matrix(result, row1, column2);
            break;
        case memory_allocation_error:
            printf("Error: not memory");
            break;
        case multi_error:
            printf("Error: column1 != row2 ");
            break;
        }
    }
    else
    {
        printf("Input error!");
        return 0;
    }
    return 0;
}
