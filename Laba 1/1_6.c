#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *sc = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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
int my_strlen(char s[])
{
    int len = 0;
    for (int i = 0; s[i] != '\0'; i++)
    {
        len++;
    }

    return len;
}

int readsym(char s)
{
    if ((s > 32 && s <= 47 && s != '-') || (s >= 58 && s <= 64) || (s >= 91 && s <= 96) || (s >= 123))
        return 1;
    else
        return 0;
}

int find_base(char *num, int len)
{
    int r = 0;
    int flag = 0;
    if (num[0] == '-')
    {
        flag = 1;
    }
    for (int i = flag; i < len; i++)
    {
        if (num[i] >= sc[r])
        {
            if (num[i] >= '0' && num[i] <= '9')
                r = num[i] - 48;

            if (num[i] >= 'A' && num[i] <= 'Z')
                r = num[i] - 55;
        }
    }
    r += 1;
    return r;
}

int to_dec(char *num, int r)
{
    int len = my_strlen(num) - 1;
    int res = 0, base = 1;
    int flag = 1;

    for (int i = len; i >= 0; i--)
    {
        if (num[i] >= '0' && num[i] <= '9')
        {
            res += (num[i] - 48) * base;
            base *= r;
        }
        if (num[i] >= 'A' && num[i] <= 'Z')
        {
            res += (num[i] - 55) * base;
            base *= r;
        }
        if (num[i] == '-')
        {
            flag = -1;
        }
    }
    res *= flag;
    return res;
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
        if (num[i] >= 'a' && num[i] <= 'z' || readsym(num[i]))
        {
            return 0;
        }
    }

    return 1;
}

int readfile(FILE **file, FILE **file_output)
{
    char sym;
    char c;
    char num[200];
    int i = 0;
    int base = 0, numdec = 0;

    while ((sym = fgetc(*file)) != EOF)
    {
        while (sym == ' ' || sym == '\t' || sym == '\n')
        {
            sym = fgetc(*file);
        }
        while (my_isdigit(sym) || my_isalpha(sym) || readsym(sym) || sym == '-')
        {
            num[i] = sym;
            i++;
            sym = fgetc(*file);
        }
        num[i] = '\0';

        printf("%s\n", num);

        if (valid(num, i) == 1)
        {
            base = find_base(num, i);
            numdec = to_dec(num, base);
            fprintf(*file_output, "%s %d %d\n", num, base, numdec);
        }

        i = 0;
    }

    return 1;
}

int main()
{

    FILE *file = fopen("inputfile.txt", "r");
    FILE *file_output = fopen("outputfile.txt", "w");

    if (file == NULL || file_output == NULL)
    {
        printf("Error opening file\n");
        return -1;
    }

    readfile(&file, &file_output);

    fclose(file);
    fclose(file_output);

    printf("Great!!!");

    return 0;
}