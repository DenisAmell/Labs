#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    int value;
    char *name;
} MemoryCell;

typedef MemoryCell T;
typedef struct
{
    T **list;
    int len;
} List;

typedef enum
{
    OK,
    NOT_MEMORY,
    ERROR_OPENING_FILE,
    NO_VALID_DATA,
    LIST_NULL,
    NO_SEARCH,
    INPUT_ERROR,
} Errors;

int readString(FILE *file, char **result, const char *separators, char *metSeparator)
{
    int isEOF = 0;
    int resCapacity = 1;
    char *res = (char *)malloc(sizeof(char) * resCapacity);
    if (res == NULL)
        return NOT_MEMORY;
    char c;
    int count = 0;
    while (1)
    {
        c = fgetc(file);
        if (c == EOF)
        {
            isEOF = 1;
            break;
        }
        int x = 0;
        int isBreak = 0;
        while (separators[x] != '\0')
        {
            if (c == separators[x])
            {
                if (metSeparator != NULL)
                    *metSeparator = c;
                isBreak = 1;
                break;
            }
            x++;
        }
        if (isBreak)
            break;
        if (count == resCapacity)
        {
            resCapacity *= 2;
            char *newRes = (char *)realloc(res, sizeof(char) * resCapacity);
            if (newRes == NULL)
            {
                free(res);
                return NOT_MEMORY;
            }
            res = newRes;
        }
        res[count] = c;
        count++;
    }
    if (count == 0)
    {
        free(res);
        return NO_VALID_DATA;
    }
    char *newRes = (char *)realloc(res, sizeof(char) * (count + 1));
    if (newRes == NULL)
    {
        free(res);
        return NOT_MEMORY;
    }
    res = newRes;
    res[count] = '\0';
    *result = res;
    if (isEOF)
        return -1;
    return OK;
}

int listCreate(List **list)
{
    *list = (List *)malloc(sizeof(List));
    if ((*list) == NULL)
    {
        return NOT_MEMORY;
    }
    (*list)->len = 0;
    return OK;
}

void listPrint(List *list)
{
    printf("------------------------------\n");
    for (int i = 0; i < list->len; i++)
    {
        printf("%s = %d\n", list->list[i]->name, list->list[i]->value);
    }
    printf("------------------------------\n");
}

int listAdd(List *list, char *name, int value, T **result)
{
    if (list == NULL || name == NULL)
    {
        return LIST_NULL;
    }
    char *key = name;
    int low = 0;
    int high = list->len - 1;
    int isFound = 0;
    int mid;

    while (low <= high)
    {
        mid = (low + high) >> 1;
        char *midVal = list->list[mid]->name;

        int cmp = strcmp(midVal, key);
        if (cmp < 0)
        {
            low = mid + 1;
        }
        else if (cmp > 0)
        {
            high = mid - 1;
        }
        else
        {
            isFound = 1;
            break;
        }
    }
    if (isFound)
        return 2;

    T *new = (T *)malloc(sizeof(T));
    if (new == NULL)
    {
        return NOT_MEMORY;
    }
    new->name = name;
    new->value = value;

    T **newList;
    if (list->len == 0)
    {
        newList = (T **)malloc(sizeof(T *));
    }
    else
    {
        newList = (T **)realloc(list->list, sizeof(T *) * (list->len + 1));
    }
    if (newList == NULL)
    {
        free(new);
        return NOT_MEMORY;
    }
    list->list = newList;
    // low - index where insert new
    if (low != list->len)
    {
        memmove(list->list + low + 1, list->list + low, sizeof(list->list) * (list->len - low));
    }
    list->list[low] = new;
    list->len++;
    if (result != NULL)
        *result = new;
    return OK;
}

T *listSearch(List *list, char *name)
{
    char *key = name;
    int low = 0;
    int high = list->len - 1;
    int isFound = 0;
    int mid;

    while (low <= high)
    {
        mid = (low + high) >> 1;
        char *midVal = list->list[mid]->name;

        int cmp = strcmp(midVal, key);
        if (cmp < 0)
        {
            low = mid + 1;
        }
        else if (cmp > 0)
        {
            high = mid - 1;
        }
        else
        {
            isFound = 1;
            break;
        }
    }
    if (isFound)
        return list->list[mid];
    return NULL;
}

void listDestroy(List *list)
{
    if (list == NULL)
        return;
    if (list->len != 0)
    {
        for (int x = 0; x < list->len; x++)
        {
            free(list->list[x]->name);
            free(list->list[x]);
        }
        free(list->list);
    }
    free(list);
}

int isContainPrint(const char *string)
{
    char print[] = "print";
    for (int x = 0; x < 5; x++)
    {
        if (string[x] != print[x])
            return 0;
    }
    return 1;
}

int print(List *list, char *string)
{ // string - "print" or "print var"
    if (string[5] == '\0')
    {
        for (int x = 0; x < list->len; x++)
        {
            printf("%s = %d\n", list->list[x]->name, list->list[x]->value);
        }
    }
    else
    {
        int strLen = strlen(string);
        char name[strLen - 5];
        name[strLen - 6] = '\0';
        memcpy(name, string + 6, sizeof(char) * (strLen - 6));
        T *data = listSearch(list, name);
        if (data == NULL)
            return NOT_MEMORY;
        printf("%s = %d\n", name, data->value);
    }
    return OK;
}

int parseInt(const char *string)
{ // correct number in str
    int x = 0;
    while (string[x] == '0')
        x++;
    if (string[x] == '\0')
        return 0;
    int num = (string[x] - '0');
    x++;
    while (string[x] != '\0')
    {
        num = num * 10 + (string[x] - '0');
        x++;
    }
    return num;
}

int parseValue(int *result, List *list, char *string)
{
    if (string[0] >= '0' && string[0] <= '9')
    {
        *result = parseInt(string);
    }
    else
    {
        T *var = listSearch(list, string);
        if (var == NULL)
        {
            listDestroy(list);
            return NO_SEARCH;
        }
        *result = var->value;
    }
    return OK;
}

int nextLine(FILE *file)
{
    char c;
    int isEOF = 0;
    while (1)
    {
        c = fgetc(file);
        if (c == EOF)
        {
            isEOF = 1;
            break;
        }
        else if (c == '\n')
        {
            break;
        }
    }
    if (isEOF)
        return -1;
    return OK;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Input error");
        return INPUT_ERROR;
    }

    FILE *data;
    List *list;

    if ((data = fopen(argv[1], "r")) == NULL)
    {
        printf("Error opening file");
        return ERROR_OPENING_FILE;
    }

    int statusCode = 0;
    char *string;
    listCreate(&list);

    while (statusCode != -1)
    {
        statusCode = readString(data, &string, "=;", NULL);
        if (statusCode != OK)
        {
            listDestroy(list);
            fclose(data);
            return statusCode;
        }
        if (isContainPrint(string))
        {
            print(list, string);
            statusCode = nextLine(data);
            continue;
        }
        char *name = string;
        T *var = listSearch(list, name);
        if (var == NULL)
        {
            var = (T *)name; // not null for listAdd
            statusCode = listAdd(list, name, 0, &var);
            if (statusCode != OK)
            {
                listDestroy(list);
                free(name);
                fclose(data);
                return statusCode;
            }
        }
        else
        {
            free(string);
        }

        char separator;
        statusCode = readString(data, &string, "+-*/%;", &separator);
        if (statusCode != OK)
        {
            listDestroy(list);
            fclose(data);
            return statusCode;
        }
        if (separator == ';')
        { // присвоение одной[го] переменной / числа
            int value;
            statusCode = parseValue(&value, list, string);
            free(string);
            if (statusCode != OK)
            {
                listDestroy(list);
                fclose(data);
                return statusCode;
            }
            var->value = value;
            statusCode = nextLine(data);
            continue;
        }
        int val1, val2;
        statusCode = parseValue(&val1, list, string);
        free(string);
        if (statusCode != OK)
        {
            listDestroy(list);
            fclose(data);
            return statusCode;
        }
        statusCode = readString(data, &string, ";", NULL);
        if (statusCode != OK)
        {
            listDestroy(list);
            fclose(data);
            return statusCode;
        }
        statusCode = parseValue(&val2, list, string);
        free(string);
        if (statusCode != 0)
        {
            listDestroy(list);
            fclose(data);
            return statusCode;
        }
        switch (separator)
        {
        case '+':
            var->value = val1 + val2;
            break;
        case '-':
            var->value = val1 - val2;
            break;
        case '*':
            var->value = val1 * val2;
            break;
        case '/':
            if (val2 == 0)
            {
                printf("Division by zero");
                listDestroy(list);
                return NO_VALID_DATA;
            }
            var->value = val1 / val2;
            break;
        case '%':
            var->value = val1 % val2;
            break;
        }
        statusCode = nextLine(data);
    }

    fclose(data);
    listDestroy(list);
    return OK;
}