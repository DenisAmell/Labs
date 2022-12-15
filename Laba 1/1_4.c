#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_file(FILE **file, FILE **file_final)
{
    unsigned str_size = 15;
    unsigned str_id = 0;

    char *str = (char *)malloc(sizeof(char) * str_size);
    if (str == NULL)
    {

        printf("Error: not memory");
        return -1;
    }
    char sym = fgetc(*file);

    if (feof(*file))
    {
        return -3;
    }

    while (sym != '\n')
    {
        if (str_id >= str_size - 1)
        {
            str_size <<= 1;
            str = realloc(str, sizeof(char) * str_size);
            if (str == NULL)
            {
                return -1;
            }
        }
        str[str_id++] = sym;
        sym = fgetc(*file);
    }
    // static
    str[str_id++] = '\0';
    char *line1, *line2, *line3;
    char *piece = strtok(str, " ");
    line1 = piece;
    piece = strtok(NULL, " ");
    line2 = piece;
    piece = strtok(NULL, " ");
    line3 = piece;
    fprintf(*file_final, "%s %s %s\n", line3, line1, line2);
    free(str);
}

int main()
{
    FILE *file = fopen("test.txt", "r");
    FILE *file_final = fopen("text2.txt", "w");
    int c;

    if ((file == NULL) || (file_final == NULL))
    {
        printf("Error\n");
        return -1;
    }

    while (1)
    {
        c = read_file(&file, &file_final);
        if (c == -3)
        {
            break;
        }
    }
    fclose(file);
    fclose(file_final);

    if (remove("test.txt") == -1)
    {
        return -2;
    }
    if (rename("text2.txt", "test.txt") != 0)
    {
        return -3;
    }

    printf("Great!!!");

    return 0;
}