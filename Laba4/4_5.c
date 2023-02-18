#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define SIZE 255

typedef struct String
{
    char *str;
    int c_sz;
    int r_sz;
} String;

typedef struct Node_t
{
    char data;
    struct Node_t *next;
} Node_t;

typedef struct Node_int
{
    int data;
    struct Node_int *next;
} Node_int;

typedef enum
{
    OK,
    NOT_MEMORY,
    ERROR_OPENING_FILE,
    INPUT_ERROR,
    NOT_VALID_DATA,
    STAK_HULL,
    PROBLEM_WITH_BRACKET,
    SOME_PROBLEM,
    PROBLEM_WITH_ZERO,
    PROBLEM_WITH_PROC,
    FILE_NULL,
} Errors;

int operation(char symbol)
{
    return (symbol == '-' || symbol == '+' || symbol == '*' ||
            symbol == '/' || symbol == '%' || symbol == '^');
}

int prior(char symbol)
{
    if (symbol == '+' || symbol == '-')
        return 2;
    else if (symbol == '*' || symbol == '/' || symbol == '%')
        return 3;
    else if (symbol == '^')
        return 4;
    else
        return -1;
}

int calculate(int first, int second, char operation, int *result)
{
    if (operation == '+')
    {
        *result = first + second;
        return 1;
    }
    else if (operation == '-')
    {
        *result = first - second;
        return 1;
    }
    else if (operation == '*')
    {
        *result = first * second;
        return 1;
    }
    else if (operation == '/')
    {
        if (second == 0)
        {
            return NOT_VALID_DATA;
        }
        *result = first / second;
        return 1;
    }
    else if (operation == '%')
    {
        if (second == 0)
        {
            return NOT_VALID_DATA;
        }
        *result = first % second;
        return 1;
    }
    else if (operation == '^')
    {
        *result = pow(first, second);
        return 1;
    }
    return 0;
}

void create_str(String *str)
{
    str->c_sz = SIZE;
    str->r_sz = 0;
    str->str = (char *)malloc(sizeof(char) * SIZE);
}

int check_size(String *str)
{
    char *tmp;
    if (str->r_sz == str->c_sz - 1)
    {
        str->c_sz *= 2;
        tmp = (char *)realloc(str->str, sizeof(char));
        if (tmp == NULL)
        {
            return NOT_MEMORY;
        }
        str->str = tmp;
    }
    return OK;
}

int push(Node_t **head, char elem)
{
    Node_t *tmp = (Node_t *)malloc(sizeof(Node_t));
    if (tmp == NULL)
    {
        return NOT_MEMORY;
    }
    tmp->next = *head;
    tmp->data = elem;
    *head = tmp;
    return OK;
}

int pop(Node_t **head, char *elem)
{
    Node_t *old;
    if (*head == NULL)
    {
        return STAK_HULL;
    }
    old = *head;
    *elem = (*head)->data;
    *head = (*head)->next;
    free(old);
    return OK;
}

int push_int(Node_int **head, int elem)
{
    Node_int *tmp = malloc(sizeof(Node_t));
    if (tmp == NULL)
    {
        return NOT_MEMORY;
    }
    tmp->next = *head;
    tmp->data = elem;
    *head = tmp;
    return OK;
}

int pop_int(Node_int **head, int *elem)
{
    Node_int *old;
    if (*head == NULL)
    {
        return STAK_HULL;
    }
    old = *head;
    *elem = (*head)->data;
    *head = (*head)->next;
    free(old);
    return OK;
}

int check_str(String *str)
{
    if (strlen(str->str) == 0)
        return FILE_NULL;
    int count_brc = 0;
    if ((!(isdigit(str->str[0]))) && (str->str[0] != '('))
    {
        return INPUT_ERROR;
    }
    if ((!(isdigit(str->str[(str->r_sz) - 2]))) && (str->str[(str->r_sz) - 2] != ')'))
    {
        return INPUT_ERROR;
    }
    for (int i = 0; i < (str->r_sz) - 1; i++)
    {
        if (str->str[i] == '(')
        {
            if ((!isdigit(str->str[i + 1]) || isdigit(str->str[i - 1])) && (str->str[i + 1] != '(' && str->str[i - 1] != '('))
            {
                return INPUT_ERROR;
            }
            count_brc++;
        }
        else if (str->str[i] == ')')
        {
            if (count_brc == 0)
            {
                return PROBLEM_WITH_BRACKET;
            }
            if ((isdigit(str->str[i + 1]) || !isdigit(str->str[i - 1])) && (str->str[i + 1] != ')' && str->str[i - 1] != ')'))
            {
                return INPUT_ERROR;
            }
            count_brc--;
        }
        else if (operation(str->str[i]))
        {
            if ((!(isdigit(str->str[i + 1]))) && (str->str[i + 1] != '('))
            {
                return INPUT_ERROR;
            }
        }
        else if (!isdigit(str->str[i]))
        {
            return INPUT_ERROR;
        }
    }
    if (count_brc == 0)
    {
        return OK;
    }
    else
    {
        return PROBLEM_WITH_BRACKET;
    }
}
int read_str(String *str)
{
    char symb;
    while ((symb = getchar()) != '\n')
    {
        if (symb == EOF)
        {
            return 2;
        }
        check_size(str);
        str->str[str->r_sz++] = symb;
    }
    str->str[str->r_sz++] = '\0';
    return 0;
}

int read_str_file(String *str, FILE *fp)
{
    char c;
    while (!feof(fp))
    {
        c = fgetc(fp);
        if (c == EOF)
        {
            if (check_size(str) == NOT_MEMORY)
            {
                return NOT_MEMORY;
            }

            str->str[str->r_sz++] = '\0';
            return 2;
        }
        if (c == '\n' || c == '\v' || c == '\r')
        {
            if (check_size(str) == NOT_MEMORY)
            {
                return NOT_MEMORY;
            }
            str->str[str->r_sz++] = '\0';
            return 0;
        }
        check_size(str);
        str->str[str->r_sz++] = c;
    }
    str->str[str->r_sz++] = '\0';

    return 0;
}

int count_expr(String str, int *result)
{
    Node_int *head = NULL;

    int i = 0;
    int tmp_res = 0;
    int first = 0, second = 0;
    int calculate_res;

    while (str.str[i] != '\0')
    {
        if (isdigit(str.str[i]))
        {
            tmp_res = tmp_res * 10 + ((str.str[i]) - '0');
        }
        else if (str.str[i] == ' ')
        {
            push_int(&head, tmp_res);
            tmp_res = 0;
        }
        else if (operation(str.str[i]))
        {
            pop_int(&head, &second);
            pop_int(&head, &first);
            calculate_res = calculate(first, second, str.str[i++], result);
            switch (calculate_res)
            {
            case 1:
                break;
            case NOT_VALID_DATA:
                return NOT_VALID_DATA;
            default:
                return SOME_PROBLEM;
            }
            push_int(&head, *result);
        }
        i++;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Input error");
        return INPUT_ERROR;
    }

    String result;
    char rubbish;
    char rubbish_stack;
    create_str(&result);

    char elem;
    Node_t *head = NULL;
    String str;
    FILE *file, *f_in;
    int status_code;
    int count_expression;

    if ((f_in = fopen("result.txt", "w")) == NULL)
    {
        printf("Error opening file");
        return ERROR_OPENING_FILE;
    }

    for (int i = 1; i < argc; i < i++)
    {
        if ((file = fopen(argv[i], "r")) == NULL)
        {
            printf("Error opening file %s\n", argv[i]);
            return ERROR_OPENING_FILE;
        }
        int index_str = 0;
        fprintf(f_in, "%s\n", argv[i]);
        printf("FileName: %s\n\n", argv[i]);

        while (!feof(file))
        {

            push(&head, '(');
            create_str(&str);
            create_str(&result);
            read_str_file(&str, file);

            status_code = check_str(&str);
            switch (status_code)
            {
            case INPUT_ERROR:
                fprintf(f_in, "%d: Wrong input str: %s\n", index_str, str.str);
                break;
            case PROBLEM_WITH_BRACKET:
                fprintf(f_in, "%d: Problem with bracket\n", index_str);
                break;
            case FILE_NULL:
                continue;
            default:
                for (int i = 0; i < str.r_sz; i++)
                {
                    if (isdigit(str.str[i]))
                    {
                        check_size(&result);
                        result.str[result.r_sz++] = str.str[i];
                    }
                    else
                    {
                        if (i != 0 && isdigit(str.str[i - 1]))
                        {
                            result.str[result.r_sz++] = ' ';
                        }
                        if (str.str[i] == '(')
                        {
                            push(&head, '(');
                        }
                        else if (str.str[i] == ')' || str.str[i] == '\0')
                        {
                            while (head->data != '(')
                            {
                                check_size(&result);
                                pop(&head, &result.str[result.r_sz++]);
                                check_size(&result);
                                result.str[result.r_sz++] = ' ';
                            }
                            pop(&head, &rubbish);
                        }
                        else if (operation(str.str[i]))
                        {
                            while (prior(str.str[i]) <= prior(head->data))
                            {
                                check_size(&result);
                                pop(&head, &result.str[result.r_sz++]);
                                check_size(&result);
                                result.str[result.r_sz++] = ' ';
                            }
                            push(&head, str.str[i]);
                        }
                    }
                }
                pop(&head, &rubbish_stack);
                result.str[result.r_sz++] = '\0';

                int *calculated = (int *)malloc(sizeof(int));
                if (calculated == NULL)
                {
                    printf("Out of memory");
                    return NOT_MEMORY;
                }
                count_expression = count_expr(result, calculated);
                if (count_expression == NOT_VALID_DATA)
                {
                    fprintf(f_in, "%d: Division by zero %%0\n", index_str);
                }
                else if (count_expression == SOME_PROBLEM)
                {
                    fprintf(f_in, "%d: Some problem\n", index_str);
                }
                else
                {
                    printf("String: %s\n", str.str);
                    printf("Reverse polish notation: %s\n", result.str);
                    printf("Result: %d\n", *calculated);
                }
                break;
            }
            free(str.str);
            free(result.str);
            index_str++;
        }
        if (index_str == 0)
        {
            printf("File %s NULL\n", argv[i]);
        }
        fprintf(f_in, "\n\n");
        printf("\n\n");
        fclose(file);
    }
    fclose(f_in);
    return OK;
}