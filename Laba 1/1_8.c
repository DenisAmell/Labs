#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const char *ErrorNames[] = {
    "Not opening file",
    "Out of Memory",
    "Not this index",
    "Error input"};

enum Errors
{
    OPENING_FILE = 1,
    OUT_OF_MEMORY,
    NOT_INDEX,
    WRONG_INPUT
};

int my_atoi(char s[])
{
    int n = 0, flag = 1;
    int j = 0;
    if (s[0] == '-')
    {
        flag = -1;
        j++;
    }
    for (int i = j; s[i] >= '0' && s[i] <= '9'; i++)
    {
        n = 10 * n + (s[i] - '0');
    }
    n *= flag;
    return n;
}

int my_isalpha(char s)
{
    if ((s >= 'A' && s <= 'Z') || (s >= 'a' && s <= 'z'))
        return 1;
    else
        return 0;
}

int my_isdigit(char s)
{
    if (s >= '0' && s <= '9')
        return 1;
    else
        return 0;
}

void print(int *res, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", res[i]);
    }
    printf("\n");
}

int func_a(int *res, int *arr, int size)
{
    int k = 0;
    for (int i = 0; i < size; i++)
    {
        if (i % 2 == 0)
        {
            res[k] = arr[i];
            k++;
        }
    }
    return k;
}

int func_b(int *res, int *arr, int size)
{
    int k = 0;
    for (int i = 0; i < size; i++)
    {
        if (arr[i] % 2 == 0)
        {
            res[k] = arr[i];
            k++;
        }
    }
    return k;
}

int func_c(int number, int *arr, int size)
{
    int res = 0;
    int max = 0;
    for (int i = 0; i < size; i++)
    {
        int tmp = abs(arr[number] - arr[i]);
        if (tmp > max)
        {
            max = tmp;
            res = arr[i];
        }
    }
    return res;
}

int func_d(int index, int *arr, int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        if (i < index)
        {
            sum += arr[i];
        }
    }
    return sum;
}

int func_e(int index, int *arr, int size)
{
    int sum = 0;

    for (int i = 0; i < size; i++)
    {
        if (arr[i] < arr[index])
        {
            sum += arr[i];
        }
    }

    return sum;
}

int readsym(char s)
{
    if ((s > 32 && s <= 47 && s != '-') || (s >= 58 && s <= 64) || (s >= 91 && s <= 96) || (s >= 123))
        return 1;
    else
        return 0;
}

int valid(char *num, int len)
{
    int minus_count = 0;
    for (int i = 0; i < len; i++)
    {
        if (num[i] == '-')
        {
            minus_count++;
        }
        if (minus_count > 1)
        {
            return 0;
        }
        else if (minus_count == 1 && num[0] != '-')
        {
            return 0;
        }
        if (num[i] < '0' && num[0] > '9' || num[i] >= 'A' && num[i] <= 'z' || readsym(num[i]))
        {
            return 0;
        }
    }

    return 1;
}

int readfile(FILE *file, int *res)
{
    char sym;
    char num[200];
    int number;
    int i = 0;
    int k = 0;

    while ((sym = fgetc(file)) != EOF)
    {
        while (sym == ' ' || sym == '\t' || sym == '\n')
        {
            sym = fgetc(file);
        }
        while (my_isdigit(sym) || my_isalpha(sym) || readsym(sym) || sym == '-')
        {
            num[i] = sym;
            i++;
            sym = fgetc(file);
        }
        num[i] = '\0';

        if (valid(num, i) == 1)
        {
            number = my_atoi(num);
            res[k] = number;
            k++;
        }

        i = 0;
    }
    return k;
}

int main()
{

    FILE *file = fopen("1_8.txt", "r");
    if (file == NULL)
    {
        printf(ErrorNames[OPENING_FILE - 1]);
        return -1;
    }
    int size_max = 128, index;
    int size_res;
    int *arr = (int *)malloc(sizeof(int) * size_max);
    int *res = (int *)malloc(sizeof(int) * size_max);

    if (arr == NULL || res == NULL)
    {
        printf(ErrorNames[OUT_OF_MEMORY - 1]);
        return -1;
    }

    int size_nuw = readfile(file, arr);

    printf("1. Numbers in odd positions\n");
    printf("2. Numbers in even positions\n");
    printf("3. The most distant by value\n");
    printf("4. The sum of the elements that precede it\n");
    printf("5. The sum of the elements that are smaller than it.\n");
    printf("Enter: ");
    int input;
    scanf("%d", &input);
    switch (input)
    {
    case 1:
        printf("Array: ");
        print(arr, size_nuw);
        printf("Result: ");
        printf("%d\n", size_res);
        size_res = func_a(res, arr, size_nuw);
        print(res, size_res);
        break;
    case 2:
        printf("Array: ");
        print(arr, size_nuw);
        printf("Result: ");
        size_res = func_b(res, arr, size_nuw);
        print(res, size_res);
        break;
    case 3:
        printf("Enter index: ");
        scanf("%d", &index);
        if (index > size_nuw - 1)
        {
            printf(ErrorNames[NOT_INDEX - 1]);
            return 0;
        }
        printf("Array: ");
        print(arr, size_nuw);
        printf("Result: %d", func_c(index, arr, size_nuw));
        break;
    case 4:
        printf("Enter index: ");
        scanf("%d", &index);
        if (index > size_nuw - 1)
        {
            printf(ErrorNames[NOT_INDEX - 1]);
            return 0;
        }
        printf("Array: ");
        print(arr, size_nuw);
        printf("Result: %d", func_d(index, arr, size_nuw));
        break;
    case 5:
        printf("Enter index: ");
        scanf("%d", &index);
        if (index > size_nuw - 1)
        {
            printf(ErrorNames[NOT_INDEX - 1]);
            return 0;
        }
        printf("Array: ");
        print(arr, size_nuw);
        printf("Result: %d", func_d(index, arr, size_nuw));
        break;
    default:
        printf(ErrorNames[WRONG_INPUT - 1]);
    }

    fclose(file);
    free(arr);
    free(res);

    return 0;
}
