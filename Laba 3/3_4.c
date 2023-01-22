// Структура message содержит следующие поля: id сообщения
// (натуральное число), текст сообщения (строка), длина сообщения в байтах
// (целое неотрицательное число).
// Приложение предлагает пользователю ввести
// сообщение, после чего относительно введённых данных [5] происходит
// заполнение переменной (объекта) структуры и запись данных этого объекта в
// файл со структурой CSV (имя файла должно генерироваться псевдослучайно:
// в имени файла каждый символ равновероятно может являться символом
// буквы или символом цифры; символы букв равновероятны, символы цифр
// равновероятны; расширение файла - “.csv”). Когда пользователь вводит
// сообщение, текст которого передаётся приложению как аргумент командной
// строки (непустая строка), ввод данных завершается, и приложение должно
// считать данные из записанного файла в динамический массив структур типа
// message и вывести их на консоль; вывод на консоль данных из переменной
// структуры типа message организуйте при помощи своей функции. [1] [2] [7]

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

enum ERRORS
{
    NOT_MEMORY = -1,
    INPUT_ERROR = -2,
    FILE_ERROR = -3,
};

typedef enum Errors
{
    ok,
    not_memory,
} Error;

typedef struct message
{
    unsigned id;
    char *data;
    unsigned size;
    unsigned len;
} message;

int create_name(Error *status_code, char **name_of_file, int len)
{

    *name_of_file = (char *)malloc(sizeof(char) * len);
    if (!*name_of_file)
    {
        *status_code = not_memory;
        return -1;
    }

    int var = 0;
    for (int i = 0; i < len; i++)
    {
        var = rand() & 1;
        switch (var)
        {
        case 0:
            (*name_of_file)[i] = rand() % 26 + 97;
            break;
        case 1:
            (*name_of_file)[i] = rand() % 10 + 48;
            break;
        }
    }
    (*name_of_file)[len] = '\0';
    return 0;
}

int init_message(message **mes)
{
    *mes = (message *)malloc(sizeof(message));

    if (mes == NULL)
    {
        return NOT_MEMORY;
    }

    (*mes)->id = 1;
    (*mes)->size = 2;
    (*mes)->data = (char *)malloc((*mes)->size * sizeof(char));
    if (!(*mes)->size)
    {
        return NOT_MEMORY;
    }
    (*mes)->len = 0;
    return 0;
}

int renew_message(message **mes)
{

    free((*mes)->data);

    (*mes)->id++;
    (*mes)->size = 2;
    (*mes)->data = (char *)malloc((*mes)->size * sizeof(char));
    if (!(*mes)->size)
    {
        return NOT_MEMORY;
    }
    (*mes)->len = 0;
    return 0;
}

int add_to_file(FILE *file_name, message **mes, char *stop_word, int *size)
{
    char sym;
    int flag = 1;
    char *tmp;
    while (flag)
    {
        sym = getchar();
        if (sym != '\n')
        {
            if ((*mes)->size == (*mes)->len)
            {
                (*mes)->size <<= 1;
                tmp = (char *)realloc((*mes)->data, (*mes)->size);
                if (tmp == NULL)
                {
                    free(tmp);
                    return NOT_MEMORY;
                }
                (*mes)->data = tmp;
            }
            (*mes)->data[(*mes)->len++] = sym;
        }
        else
        {
            if ((*mes)->size == (*mes)->len)
            {
                (*mes)->size += 1;
                tmp = (char *)realloc((*mes)->data, (*mes)->size);
                if (tmp == NULL)
                {
                    free(tmp);
                    return NOT_MEMORY;
                }
                (*mes)->data = tmp;
            }
            (*mes)->data[(*mes)->len] = '\0';
            if (!strcmp((*mes)->data, stop_word))
            {
                flag = 0;
                *size = (*mes)->id;
                free((*mes)->data);
                free((*mes));
                if (init_message(mes) == NOT_MEMORY)
                    return NOT_MEMORY;
                break;
            }
            fprintf(file_name, "%d,", (*mes)->id);
            fprintf(file_name, "\"%s\"\n", (*mes)->data);
            if (renew_message(mes) == NOT_MEMORY)
            {
                return NOT_MEMORY;
            }
        }
    }
    return 0;
}

int from_file_to_message(FILE *file_name, message ***mes, int size)
{
    int del = 1;
    int buf_size = 2, curr = 0, ln = 0, num = 0;
    char sym;
    char *buff = NULL;
    char *tmp;

    *mes = (message **)malloc(size * sizeof(message *));
    if (!*mes)
    {
        return NOT_MEMORY;
    }
    while ((sym = fgetc(file_name)) != EOF)
    {
        if (sym == ',' && del)
        {
            init_message(&(*mes)[curr]);
            if (buf_size == ln)
            {
                buf_size += 1;
                buff = (char *)realloc(buff, buf_size);
                if (!buff)
                {
                    free(buff);
                    return NOT_MEMORY;
                }
            }
            buff[ln] = '\0';
            num = atoi(buff);
            (*mes)[curr]->id = num;
            free(buff);
            buff = NULL;
            buf_size = 2;
            ln = 0;
            del = 0;
            fgetc(file_name);
        }
        else if (del)
        {
            if (!buff)
            {
                buff = (char *)malloc(buf_size * sizeof(buff));
                if (!buff)
                {
                    return NOT_MEMORY;
                }
            }
            if (buf_size == ln)
            {
                buf_size <<= 1;
                char *temp;
                if (!(temp = (char *)realloc(buff, buf_size * sizeof(char))))
                {
                    free(buff);
                    return NOT_MEMORY;
                }
                buff = temp;
            }
            buff[ln++] = sym;
        }
        else if (!del && sym != '\n')
        {
            if ((*mes)[curr]->size - 1 == (*mes)[curr]->len)
            {
                (*mes)[curr]->size <<= 1;
                tmp = (char *)realloc((*mes)[curr]->data, (*mes)[curr]->size * sizeof(char));
                if (!tmp)
                {
                    free(tmp);
                    return NOT_MEMORY;
                }
                (*mes)[curr]->data = tmp;
            }
            (*mes)[curr]->data[(*mes)[curr]->len++] = sym;
        }
        else if (sym == '\n')
        {
            (*mes)[curr]->data[(*mes)[curr]->len - 1] = '\0';
            curr++;
            del = 1;
        }
    }
    return 0;
}

int print_messages(message ***mes, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("id: %d, message: %s\n", (*mes)[i]->id, (*mes)[i]->data);
    }
    return 0;
}

int free_messages(message ***mes, int len)
{
    for (int i = 0; i < len; i++)
    {
        free((*mes)[i]->data);
        free((*mes)[i]);
    }
    free(*mes);
    *mes = NULL;
    return 0;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    Error status_code;
    char *flag_data;
    char suffix[] = ".csv";
    char *file_name = NULL;
    message *mes = NULL;
    message **post = NULL;
    int ids;
    // while (flag_data != argv[1])
    // {
    //     scanf("%s", flag_data);
    // }

    if (argc != 2)
    {
        printf("Input Error");
        return INPUT_ERROR;
    }
    create_name(&status_code, &file_name, 5);
    if (status_code == not_memory)
    {
        printf("Out of memory");
        return NOT_MEMORY;
    }
    strcat(file_name, suffix);
    if (init_message(&mes) == NOT_MEMORY)
    {
        printf("Out of memory");
        return NOT_MEMORY;
    }
    else
    {
        printf("\nPlease, enter messages, if you want to stop, enter \'%s\'\n", argv[1]);
        FILE *file = fopen(file_name, "w");
        if (file == NULL)
        {
            printf("Error opening file");
            return FILE_ERROR;
        }
        add_to_file(file, &mes, argv[1], &ids);
        fclose(file);

        file = fopen(file_name, "r");
        int code = from_file_to_message(file, &post, ids);
        if (code == 0)
        {
            printf("\n Messages\n");
            fclose(file);
            print_messages(&post, ids);
            free_messages(&post, ids);
        }
        else
        {
            printf("Error");
            return code;
        }
    }

    return 0;
}