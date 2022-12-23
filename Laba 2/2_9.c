#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

const char *ErrorNames[] = {
    "Out of Memory",
    "Wrong system base",
    "Error input number"};

enum Errors
{
    OUT_OF_MEMORY = 0,
    BASE,
    VALID_NUMBER
};

char *sc = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int proverka(char *num, int r)
{
    int len = strlen(num);
    for (int i = 0; i < len; i++)
    {
        if (num[i] >= sc[r])
        {
            return 0;
        }
    }
    return 1;
}

int sdvig(char *str, int len, char **result)
{
    *result = NULL;
    char *res = (char *)malloc(sizeof(char) * (len));
    if (res == NULL)
    {
        return OUT_OF_MEMORY;
    }
    int k = 0;
    for (int i = 0; i < len; i++)
    {
        if (i < (len - strlen(str)))
        {
            res[i] = '0';
        }
        else
        {
            res[i] = str[k];
            k++;
        }
    }
    res[len + 1] = '\0';
    *result = res;
    return 0;
}

void sdvig2(char *res, int len)
{
    int j = 1;
    char sym;
    for (int i = 0; i < len; i++)
    {
        sym = res[j];
        res[i] = sym;
        j++;
    }
    res[len] = '\0';
}

int sum(char *result, char *s1, char *s2, int r, int len)
{

    int tmp;
    int res;
    int k = 0;
    int flag = 0;
    result[len + 1] = '\0';
    int index = len;
    char *shifted;
    if (strlen(s1) != strlen(s2))
    {
        if (strlen(s1) > strlen(s2))
        {
            /*TODO: save returned result & check it*/ sdvig(s2, len, &shifted);
            s2 = shifted;
            flag = 1;
        }
        else
        {
            flag = 0;
            /*TODO: save returned result & check it*/ sdvig(s1, len, &shifted);
            s1 = shifted;
        }
    }
    for (int i = len - 1; i >= 0; i--)
    {
        int tmp_s1 = (s1[i] > 47 && s1[i] < 58) ? s1[i] - '0' : s1[i] - 55;
        int tmp_s2 = (s2[i] > 47 && s2[i] < 58) ? s2[i] - '0' : s2[i] - 55;

        res = tmp_s1 + tmp_s2 + k;

        if (res >= r)
        {
            tmp = res - r;
            k = 1;
            result[index] = sc[tmp];
            index--;
        }
        else
        {
            k = 0;
            result[index] = sc[res];
            index--;
        }
    }
    if (k > 0)
    {
        result[index] = sc[k];
    }
    else
    {
        result[index] = sc[k];
        sdvig2(result, len);
    }
    if (flag == 1)
        free(s2);
    else
        free(s1);
}

int sum_base(int r, int n, char **result, ...)
{
    if (r < 0 || r > 36)
    {
        return -2;
    }
    va_list factor;
    va_start(factor, result);
    char *str;
    int k;
    char *tmp = NULL;

    *result = (char *)malloc(sizeof(char) * 2);
    // TODO: check if result is null
    strcpy(*result, "0");

    for (int i = 0; i < n; i++)
    {
        str = va_arg(factor, char *);
        int len = strlen(*result) >= strlen(str) ? strlen(*result) : strlen(str);

        tmp = (char *)malloc(sizeof(char) * (len + 1));
        // TODO: check if tmp is NULL

        if (proverka(str, r) == 0)
        {
            return -3;
        }
        else
        {
            k = sum(tmp, *result, str, r, len);
            // printf("%s %s\n", tmp, result);

            if (k == -1)
            {
                // TODO: clear allocated memory
                return -1;
            }

            free(*result);
            *result = tmp;
        }
    }
    va_end(factor);
    return 0;
}

int main()
{
    char *result;
    int status_code = sum_base(16, 2, &result, "00000000000000F", "3");
    if (status_code == -1)
        printf(ErrorNames[OUT_OF_MEMORY - 1]);
    else if (status_code == -2)
        printf(ErrorNames[BASE - 1]);
    else if (status_code == -3)
        printf(ErrorNames[VALID_NUMBER - 1]);
    else
        printf("%s", result);

    free(result);
    return 0;
}
