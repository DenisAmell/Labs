#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef enum Errors
{
    ok,
    not_memory,
    substr_found,
    substr_not_found,
    open_file_error,
} Error;

int find_str(Error *status_code, char *str, char *substr)
{

    int str_len = strlen(str);
    int sub_len = strlen(substr);

    int *pref = (int *)malloc(sizeof(int) * sub_len);
    if (pref == NULL)
    {
        *status_code = not_memory;
        return -1;
    }

    int i, j;
    pref[0] = 0;
    for (i = 1, j = 0; i < sub_len; i++)
    {
        while (j > 0 && substr[j] != substr[i])
        {
            j = pref[j - 1];
        }
        if (substr[j] == substr[i])
            j++;
        pref[i] = j;
    }

    for (i = 0, j = 0; i < str_len; i++)
    {
        while (j > 0 && substr[j] != str[i])
        {
            j = pref[j - 1];
        }
        if (substr[j] == str[i])
            j++;
        if (j == sub_len)
        {
            free(pref);
            *status_code = substr_found;
            return 0;
        }
    }
    free(pref);
    *status_code = ok;
    return 0;
}

int check_files(Error *status_code, char *substr, int num, ...)
{
    Error status_code2;
    FILE *f_in;
    char *file_name;
    va_list p;
    va_start(p, num);
    int is_found = 0, line = 1;
    char str[BUFSIZ]; // !!!
    int flag = 0;
    while (num != 0)
    {
        line = 1;
        num--;
        file_name = va_arg(p, char *);
        f_in = fopen(file_name, "r");
        if (f_in == NULL)
        {
            *status_code = open_file_error;
            return -1;
        }
        while (fgets(str, sizeof(str), f_in) != NULL)
        {
            find_str(&status_code2, str, substr);
            if (status_code2 == substr_found)
            {
                printf("Substr %s found in file: %s, in line=%d\n", substr, file_name, line);
                flag++;
            }
            else if (status_code2 == not_memory)
            {
                *status_code = not_memory;
                return -1;
            }
            line++;
        }
        fclose(f_in);
    }

    if (flag == 0)
    {
        *status_code = substr_not_found;
        return -1;
    }

    va_end(p);
    *status_code = ok;
    return 0;
}

int main()
{
    Error status_code;
    check_files(&status_code, "lol", 2, "f1.txt", "t1.txt");
    switch (status_code)
    {
    case substr_found:
        printf("Substr not found");
        return -1;
    case open_file_error:
        printf("Error opening file or not found file");
        return -1;
    case not_memory:
        printf("Out of memory");
        return -1;
    case ok:
        printf("Great!!!");
    }
    return 0;
}