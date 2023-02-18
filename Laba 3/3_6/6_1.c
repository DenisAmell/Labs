#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>

typedef struct
{
    char *data;
    unsigned int len;
} String;

typedef enum
{
    OK,
    NOT_MEMORY,
    LEN_NULL,
} Errors;

void print_string(String *string)
{
    for (int i = 0; i < string->len; i++)
    {
        putchar(string->data[i]);
    }
    printf("\n");
}

void create_string(String **result, const char *string, int *code)
{
    int len = 0;

    char *data = NULL;

    String *res;
    if (string != NULL)
    {
        while (string[len] != '\0')
            len++;

        if (len == 0)
        {
            *code = LEN_NULL;
            return;
        }
        data = (char *)malloc(sizeof(char) * len);
        if (data == NULL)
        {
            *code = NOT_MEMORY;
            return;
        }
        memcpy(data, string, len * sizeof(char));
    }
    res = (String *)malloc(sizeof(String));
    if (res == NULL)
    {
        *code = NOT_MEMORY;
        if (data != NULL)
            free(data);
        return;
    }
    res->len = len;
    res->data = data;
    *result = res;
    return;
}

void concat_string(int *code, String **result, int count, ...)
{
    String **strings = (String **)malloc(sizeof(String *) * count);
    if (strings == NULL)
    {
        *code = NOT_MEMORY;
        return;
    }
    va_list args;
    va_start(args, count);
    unsigned int res_len = 0;

    for (int i = 0; i < count; i++)
    {
        strings[i] = va_arg(args, String *);
        res_len += strings[i]->len;
    }
    char *data = (char *)malloc(sizeof(char) * res_len);
    if (data == NULL)
    {
        *code = NOT_MEMORY;
        free(strings);
        return;
    }

    String *res = (String *)malloc(sizeof(String));
    if (res == NULL)
    {
        free(data);
        free(strings);
        *code = NOT_MEMORY;
        return;
    }
    res_len = 0;

    for (int i = 0; i < count; i++)
    {
        memcpy(data + res_len, strings[i]->data, strings[i]->len * sizeof(char));
        res_len += strings[i]->len;
    }
    res->len = res_len;
    res->data = data;
    *result = res;
    free(strings);
    return;
}

void clear_string(String *string)
{
    if (string->len == 0)
        return;
    free(string->data);
    string->len = 0;
}

int compare(char a, char b)
{
    return a - b;
}

int compare_strings(String *str1, String *str2, int (*comparator)(char, char))
{
    for (int i = 0; i < (str1->len > str2->len ? str2->len : str1->len); i++)
    {
        int code = comparator(str1->data[i], str2->data[i]);
        if (code > 0)
            return 1;
        else if (code < 0)
            return -1;
    }

    if (str1->len > str2->len)
        return 1;
    if (str1->len < str2->len)
        return -1;
    return 0;
}

void copy_string(int *code, String *string, String *dest)
{
    clear_string(dest);
    char *data = (char *)malloc(sizeof(char) * string->len);
    if (data == NULL)
    {
        *code = NOT_MEMORY;
        return;
    }
    memcpy(data, string->data, string->len * sizeof(char));
    dest->len = string->len;
    dest->data = data;
    return;
}

void free_string(String *string)
{
    clear_string(string);
    free(string);
}

void duplicate_string(int *code, String **result, String *string)
{
    int status_code;
    create_string(result, NULL, &status_code);
    switch (status_code)
    {
    case NOT_MEMORY:
        *code = NOT_MEMORY;
        return;
    case LEN_NULL:
        *code = LEN_NULL;
        return;
    }

    copy_string(&status_code, string, *result);
    if (status_code == NOT_MEMORY)
    {
        *code = NOT_MEMORY;
        free_string(*result);
        return;
    }
    return;
}

int main(int argc, char *argv[])
{

    String *s1, *s2, *s3, *s4;
    int status_code;

    create_string(&s1, "ladno", &status_code);
    if (status_code == NOT_MEMORY)
    {
        printf("Out of memory");
        return NOT_MEMORY;
    }
    create_string(&s2, "lol", &status_code);
    if (status_code == NOT_MEMORY)
    {
        free(s1);
        printf("Out of memory");
        return NOT_MEMORY;
    }
    print_string(s1);
    print_string(s2);

    concat_string(&status_code, &s3, 2, s1, s2);
    if (status_code == NOT_MEMORY)
    {
        free(s1);
        free(s2);
        printf("Out of memory");
        return NOT_MEMORY;
    }
    print_string(s3);

    duplicate_string(&status_code, &s4, s3);
    switch (status_code)
    {
    case NOT_MEMORY:
        printf("Out of memory");
        free(s1);
        free(s2);
        free(s3);
        return NOT_MEMORY;

    case LEN_NULL:
        printf("String len = 0");
        return LEN_NULL;
    default:
        print_string(s4);
        break;
    }

    printf("%d\n", compare_strings(s1, s3, compare));

    free(s1);
    free(s2);
    free(s3);
    free(s4);
    return 0;
}