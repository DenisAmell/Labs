#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    OK,
    NOT_MEMORY,
    INPUT_ERROR,
    NOT_FOUND,
} Errors;

int num_bits(int k)
{
    return 2 << (k - 1);
}

int check_bits(int l, int num, int straight)
{

    int count = 0;
    if (!straight)
    {
        while (num != 0)
        {
            count += (num & 1);
            num >>= 1;
        }
    }
    else
    {
        while (num != 0)
        {
            if (!(num & 1))
            {
                if (count == l)
                    ;
                break;
                count = 0;
            }
            else
            {
                count++;
            }
            num >>= 1;
        }
    }
    return count == l;
}

long long fact(int n)
{
    if (n > 22)
        return INPUT_ERROR;
    else if (n == 0)
        return 1;
    else
        return (long long)n * fact(n - 1);
}

int get_mem(int k, int l)
{
    long long a = fact(k), b = fact(l), c = fact(k - 1);
    return (int)(a / (b * c));
}

void bits_in(int k, int l, int **res, int *len, int *code)
{
    int lim = num_bits(k);
    int size = get_mem(k, l);
    int pred = num_bits(l) - 1;

    *res = (int *)malloc(sizeof(int) * size);
    if (res == NULL)
    {
        *code = NOT_MEMORY;
        return;
    }

    for (int i = pred; i < lim; i++)
    {
        if (check_bits(l, i, 0))
        {
            (*res)[(*len)++] = i;
        }
    }
    *code = size ? OK : NOT_FOUND;
    return;
}

void bits_forward(int k, int l, int **res, int *len, int *code)
{
    int lim = num_bits(k);
    int pred = num_bits(l) - 1;
    int size = lim - pred;

    *res = (int *)malloc(size * sizeof(int));
    if (res == NULL)
    {
        *code = NOT_MEMORY;
        return;
    }

    for (int i = pred; i < lim; i++)
    {
        if (check_bits(l, i, 1))
        {
            (*res)[(*len)++] = i;
        }
    }

    *code = size ? OK : NOT_FOUND;
    return;
}

void print_res(int *res, int ln)
{
    for (int i = 0; i < ln; i++)
    {
        printf("%d ", res[i]);
    }
    printf("\n");
}

int main()
{
    int k, l, code;
    int *res1, *res2;
    int ln1 = 0, ln2 = 0;

    printf("Enter k: ");
    if (scanf("%d", &k) != 1)
    {
        printf("Input error");
        return INPUT_ERROR;
    }
    printf("Enter l: ");
    if (scanf("%d", &l) != 1)
    {
        printf("Input error");
        return INPUT_ERROR;
    }

    if (k > 31)
    {
        printf("Overflow");
        return INPUT_ERROR;
    }
    if (l > k)
    {
        printf("\nL cannot exceed k!\n");
        return INPUT_ERROR;
    }
    if (l < 0 || k < 0)
    {
        printf("\nOnly positive numbers!\n");
        return INPUT_ERROR;
    }

    bits_in(k, l, &res1, &ln1, &code);

    switch (code)
    {
    case OK:
        printf("\nThe first array, numbers have l= %d units\n", l);
        print_res(res1, ln1);
        free(res1);
        break;

    case NOT_MEMORY:
        printf("Out of memory");
        return NOT_MEMORY;
    case NOT_FOUND:
        printf("Numbers for l=%d and k=%d werent found\n", l, k);
        break;
    }

    bits_forward(k, l, &res2, &ln2, &code);
    switch (code)
    {
    case OK:
        printf("\nThe second array, numbers have l= %d units going forward\n", l);
        print_res(res2, ln2);
        free(res2);
    case NOT_MEMORY:
        printf("Out of memory");
        return NOT_MEMORY;
    case NOT_FOUND:
        printf("Numbers for l=%d and k=%d werent found\n", l, k);
        break;
    }

    return OK;
}
