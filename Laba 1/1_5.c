#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Errors
{
    ok,
    open_file_error,
    memory_allocation_error,
    write_error

} Errors;

int get_string(Errors *status_code, int *len, char **string)
{
    *len = 0;
    int capacity = 1;
    (*string) = (char *)malloc(sizeof(char));
    if (string == NULL)
    {
        *status_code = memory_allocation_error;
        return -1;
    }

    char c = getchar();
    while (c != '\n')
    {
        (*string)[(*len)++] = c;
        if (*len >= capacity)
        {
            capacity *= 2;
            char *tmp = (char *)realloc(*string, capacity * sizeof(char));
            if (tmp == NULL)
            {
                *status_code = memory_allocation_error;
                return -1;
            }
            else
            {
                (*string) = tmp;
            }
        }
        c = getchar();
    }
    (*string)[*len] = '\0';
    return 0;
}

int writn_write(FILE **file, FILE **fout)
{
    char sym;
    int ind;
    while ((sym = fgetc(*file)) != EOF)
    {
        ind = fprintf(*fout, "%c", sym);
        if (ind < 0)
            return ind;
    }
    return 1;
}

int read_string(FILE *file, FILE *fout)
{
    FILE *fd;
    unsigned line_size = 15;
    unsigned line_ind = 0;
    char *line = (char *)malloc(sizeof(char) * line_size);
    if (line == NULL)
    {
        return -2;
    }

    char sym = fgetc(file);
    if (feof(file))
        return -3;
    while (sym != '\n')
    {
        if (line_ind >= line_size - 1)
        {
            line_size *= 2;
            char *tmp = realloc(line, line_size * sizeof(char));
            if (tmp == NULL)
            {
                free(line);
                return -2;
            }
            else
            {
                *line = *tmp;
            }
        }
        line[line_ind++] = sym;
        sym = fgetc(file);
    }
    line[line_ind++] = '\0';
    fd = fopen(line, "r");
    if (fd == NULL)
        return -1;

    if (writn_write(&fd, &fout) != 1)
        return -4;

    free(line);
    fclose(fd);
    return 0;
}

int readfile(Errors *status_code, FILE *file, FILE *fout)
{
    while (1)
    {
        int ind = read_string(file, fout);
        if (ind == -3)
        {
            *status_code = ok;
            break;
        }
        if (ind == -1)
        {
            *status_code = open_file_error;
            break;
        }

        if (ind == -2)
        {
            *status_code = memory_allocation_error;
            break;
        }
        if (ind == -4)
        {
            *status_code = write_error;
            break;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    Errors status_code;
    FILE *file, *fout;
    char *file_name;
    int file_len;
    get_string(&status_code, &file_len, &file_name);
    if (status_code == memory_allocation_error)
    {
        printf("Out of Memory");
        return -1;
    }
    fout = fopen(file_name, "w");
    if (fout == NULL)
    {
        printf("Not opening file");
        return -1;
    }

    if (argc == 3 && strcmp(argv[1], "-fi") == 0)
    {
        int value;
        int id;
        if (strcmp(file_name, argv[2]) == 0)
        {
            printf("The names matched");
            fclose(fout);
            return -1;
        }

        file = fopen(argv[2], "r");
        if (file == NULL)
        {
            fclose(fout);
            printf("Not opening file");
            return -1;
        }
        readfile(&status_code, file, fout);
        switch (status_code)
        {
        case open_file_error:
            printf("Not opening file");
            fclose(fout);
            return -1;
        case memory_allocation_error:
            printf("Out of Memory");
            fclose(fout);
            return -1;
        case write_error:
            printf("Not write in file");
            fclose(fout);
            return -1;
        default:
            fclose(file);
            fclose(fout);
            printf("Great!!!");
            return 0;
        }
    }
    else if (argc == 2 && strcmp(argv[1], "-cin") == 0)
    {
        int n;
        char c;
        if (!(scanf("%d%c", &n, &c) == 2 && c == '\n') || n < 0)
        {
            printf("Enter numbers: ");
            fclose(fout);
            return -1;
        }
        for (long i = 0; i < n; i++)
        {
            int len = 0;
            char *filename;
            int ind_write;
            get_string(&status_code, &len, &filename);
            if (status_code == memory_allocation_error)
            {

                printf("Error read file");
                fclose(fout);
                return -1;
            }
            if (strcmp(file_name, filename) == 0)
            {
                printf("The names matched");
                fclose(fout);
                return -1;
            }

            file = fopen(filename, "r");
            if (file == NULL)
            {
                printf("Not opening file");
                fclose(fout);
                return -1;
            }
            ind_write = writn_write(&file, &fout);
            if (ind_write < 0)
            {
                printf("Not write in file");
                fclose(fout);
                return -1;
            }
            free(filename);
            fclose(file);
        }
        fclose(fout);
        printf("Great");
        return 0;
    }
    else if (argc > 2 && strcmp(argv[1], "-arg") == 0)
    {
        int ind_write;
        for (int i = 2; i < argc; ++i)
        {
            if (strcmp(file_name, argv[i]) == 0)
            {
                printf("The names matched");
                fclose(fout);
                return -1;
            }
            file = fopen(argv[i], "r");
            if (file == NULL)
            {
                printf("Not opening file");
                fclose(fout);
                return -1;
            }
            ind_write = writn_write(&file, &fout);
            if (ind_write < 0)
            {
                printf("Not write in file");
                fclose(fout);
                return -1;
            }
            fclose(file);
        }
        fclose(fout);
        printf("OK!");
        return 0;
    }
    else
    {
        printf("Error input");
        return -1;
    }
}