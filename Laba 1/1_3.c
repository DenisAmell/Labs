#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static char *ErrorNames[] = {
    "Not opening file",
    "Out of Memory",
    "Not this index",
    "Wrong input flag",
    "Few arguments!",
    "Too many arguments!"};

enum Errors
{
    OPENING_FILE = 1,
    OUT_OF_MEMORY,
    NOT_INDEX,
    WRONG_INPUT,
    FEW_ARGUMENTS,
    MANY_ARGUMENTS
};

int help_for_i(FILE *file, char sym);
int help_for_s(FILE *file, char sym);
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

int readsym(char s)
{
    if ((s > 32 && s <= 47) || (s >= 58 && s <= 64) || (s >= 91 && s <= 96) || (s >= 123))
        return 1;
    else
        return 0;
}

int my_isspace(char s)
{
    if (s == '\n' || s == '\t')
        return 1;
    else
        return 0;
}

char *uppercase(char *str, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (str[i] >= 97 && str[i] <= 122)
            str[i] -= 32;
    }

    return str;
}

void to_ascii(char *str, int size, FILE *out)
{
    for (int i = 0; i < size; i++)
        fprintf(out, "%d", str[i]);
}

int func_d(FILE *file, FILE *out)
{
    char sym;
    while ((sym = fgetc(file)) != EOF)
    {
        if (!my_isdigit(sym))
            fputc(sym, out);
    }
}

int func_i(FILE *file, FILE *out)
{
    char sym;
    while ((sym = fgetc(file)) != EOF)
    {
        fprintf(out, "%d\n", help_for_i(file, sym));
    }
}
int help_for_i(FILE *file, char sym)
{
    int count = 0;
    while (sym != '\n')
    {
        if (sym == EOF)
        {
            return count;
        }
        else if (isalpha(sym))
        {
            count++;
        }
        sym = fgetc(file);
    }
    return count;
}
int func_s(FILE *file, FILE *out)
{
    char sym;
    while ((sym = fgetc(file)) != EOF)
    {
        fprintf(out, "%d\n", help_for_s(file, sym));
    }
}

int help_for_s(FILE *file, char sym)
{
    int count = 0;
    while (sym != '\n')
    {
        if (sym == EOF)
        {
            return count;
        }
        else if (readsym(sym) || my_isspace(sym))
        {
            count++;
        }
        sym = fgetc(file);
    }
    return count;
}

int func_a(FILE *file, FILE *out)
{
    char sym;
    while ((sym = fgetc(file)) != EOF)
    {
        if (!isdigit(sym))
        {
            fprintf(out, "%d", sym);
        }
        else
        {
            fprintf(out, "%c", sym);
        }
    }
}

int func_f(FILE *file, FILE *out)
{
    char sym;
    int count = 0, i = 0;
    char str[100];
    while ((sym = fgetc(file)) != EOF)
    {

        while (isspace(sym))
        {
            fprintf(out, "%c", sym);
            sym = fgetc(file);
        }
        while (my_isalpha(sym) || my_isdigit(sym) || readsym(sym))
        {
            str[i] = sym;
            i++;
            sym = fgetc(file);
        }
        str[i] = '\0';
        count++;

        if (count % 2 == 0 && count % 5 == 0)
            to_ascii(uppercase(str, i), i, out);
        else if (count % 2 == 0)
            fputs(uppercase(str, i), out);
        else if (count % 5 == 0)
            to_ascii(str, i, out);
        else
            fputs(str, out);

        fprintf(out, "%c", sym);

        i = 0;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf(ErrorNames[FEW_ARGUMENTS - 1]);
        return 0;
    }
    if (argv[1][0] != '-' && argv[1][0] != '/')
    {
        printf(ErrorNames[WRONG_INPUT - 1]);
        return -1;
    }

    else
    {
        if (argc == 3 && argv[1][1] == 'c')
        {
            printf(ErrorNames[FEW_ARGUMENTS - 1]);
            return 0;
        }
        if (argc > 4)
        {
            printf(ErrorNames[MANY_ARGUMENTS - 1]);
            return 0;
        }
    }
    // std
    char *name = argv[2];

    FILE *file = fopen(name, "r");
    FILE *fout;

    if (file == NULL)
    {
        printf(ErrorNames[OPENING_FILE - 1]);
        return -1;
    }

    if (argv[1][1] == 'n')
    {
        if (!argv[3])
        {
            printf(ErrorNames[FEW_ARGUMENTS - 1]);
            return -1;
        }
        fout = fopen(argv[3], "w");
        if (fout == NULL)
        {
            printf(ErrorNames[OPENING_FILE - 1]);
            return -1;
        }
    }
    else
    {
        char path[100] = "out_";
        strcat(path, name);
        fout = fopen(path, "w");
        if (fout == NULL)
        {
            printf(ErrorNames[OPENING_FILE - 1]);
            return -1;
        }
    }
    if (argv[1][1] == 'd' || argv[1][2] == 'd')
        func_d(file, fout);
    else if (argv[1][1] == 'i' || argv[1][2] == 'i')
        func_i(file, fout);
    else if (argv[1][1] == 's' || argv[1][2] == 's')
        func_s(file, fout);
    else if (argv[1][1] == 'a' || argv[1][2] == 'a')
        func_a(file, fout);
    else if (argv[1][1] == 'f' || argv[1][2] == 'f')
        func_f(file, fout);
    else
        printf(ErrorNames[WRONG_INPUT - 1]);

    fclose(file);
    fclose(fout);

    return 0;
}