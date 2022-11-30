#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int print_matrix(int **res, int N, int M)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            printf("%d ", res[i][j]);
        }
        printf("\n");
    }
}

void generate_matrix(int **matrix, int N, int M)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            matrix[i][j] = rand() % 201 - 100;
        }
    }
}

int proiz_matrix(int **res, int **A, int **B, int N, int M)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            res[i][j] = 0;
            for (int k = 0; k < N; k++)
            {
                res[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return print_matrix(res, N, M);
}

int **matr(int **a, int n, int x)
{
    int **res = (int **)malloc((n - 1) * sizeof(int *));
    for (int i = 0; i < n - 1; ++i)
        res[i] = (int *)malloc((n - 1) * sizeof(int));
    for (int i = 1; i < n; ++i)
        for (int j = 0, k = 0; j < n; ++j, ++k)
        {
            if (j == x)
            {
                --k;
                continue;
            }
            res[i - 1][k] = a[i][j];
        }
    return res;
}

int det(int **a, int n)
{
    if (n == 1)
        return a[0][0];
    if (n == 2)
        return (a[1][1] * a[0][0]) - (a[0][1] * a[1][0]);
    int ans = 0, sig = 1;
    for (int i = 0; i < n; ++i)
    {
        ans += (sig * a[0][i] * det(matr(a, n, i), n - 1));
        sig *= -1;
    }
    return ans;
}

int main()
{

    srand(time(0));
    int row1, column1;
    int row2, column2;

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
        scanf("%d", &row1, column1);
        if ((row1 >= 1) && (row1 <= 10) && (column1 >= 1) && (column1 <= 10))
        {
            int **A = (int **)malloc(sizeof(int *) * row1);
            for (int i = 0; i < column1; i++)
            {
                A[i] = (int *)malloc(sizeof(int) * column1);
            }
            if (A == NULL)
            {
                printf("Error: not memory");
                return -1;
            }

            generate_matrix(A, row1, column1);
            print_matrix(A, row1, column1);
            free(A);
        }
        else
        {
            printf("Enter in the range [1..10]");
            return -1;
        }
    }
    else if ((input > 1) && (input <= 3))
    {
        printf("Enter row and column for matrix 1: ");
        scanf("%d%d", &row1, &column1);
        printf("Enter row and column for matrix 2: ");
        scanf("%d%d", &row2, &column2);

        if ((row1 >= 1) && (row1 <= 10) && (column1 >= 1) && (column1 <= 10) && (row2 >= 1) && (row2 <= 10) && (column2 >= 1) && (column2 <= 10))
        {
            if (column1 != row2)
            {
                printf("Error: column1 != row2");
                return -1;
            }
            else if ((input == 3) && ((row1 != column1) || (row2 != column2)))
            {
                printf("Error: non-quadratic matrix\n");
                printf("It is not possible to calculate");
                return -1;
            }
            else
            {
                int **A = (int **)malloc(sizeof(int *) * row1);
                for (int i = 0; i < column1; i++)
                {
                    A[i] = (int *)malloc(sizeof(int) * column1);
                }
                if (A == NULL)
                {
                    printf("Error: not memory");
                    return -1;
                }

                generate_matrix(A, row1, column1);
                printf("Matrix 1: \n");
                print_matrix(A, row1, column1);
                if (input == 3)
                {
                    printf("Det Matrix 1: %d\n", det(A, row1));
                }

                int **B = (int **)malloc(sizeof(int *) * row2);
                for (int i = 0; i < column1; i++)
                {
                    B[i] = (int *)malloc(sizeof(int) * column2);
                }
                if (B == NULL)
                {
                    printf("Error: not memory");
                    return -1;
                }

                generate_matrix(B, row2, column2);
                printf("Matrix 2: \n");
                print_matrix(B, row2, column2);
                if (input == 3)
                {
                    printf("Det Matrix 2: %d\n", det(B, row2));
                }

                int **result = (int **)malloc(sizeof(int *) * row1);
                for (int i = 0; i < column2; i++)
                {
                    result[i] = (int *)malloc(sizeof(int) * column2);
                }
                if (result == NULL)
                {
                    printf("Error: not memory");
                    return -1;
                }

                printf("Matrix multiplication: \n");
                proiz_matrix(result, A, B, row1, column2);
                if (input == 3)
                {
                    printf("Det multiplication: %d\n", det(result, row1));
                }
                free(A);
                free(B);
                free(result);
            }
        }
        else
        {
            printf("Enter in the range [1..10]");
            return -1;
        }
    }
    else
    {
        printf("Input error!");
        return 0;
    }
    return 0;
}