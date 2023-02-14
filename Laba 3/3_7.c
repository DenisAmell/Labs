#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LEN 500

typedef struct
{
    unsigned int day;
    unsigned int moth;
    unsigned int year;
} Birthday;

const char *sex[] = {
    "Male",
    "Female",
};
typedef enum
{
    MALE = 0,
    FEMALE
} Gender;

typedef struct
{
    char surname[MAX_LEN];
    char name[MAX_LEN];
    char patronymic[MAX_LEN];
    Birthday birthday;
    Gender gender;
    float avg_monthy;

} Resident;

typedef struct Node Node;
typedef struct Node
{
    Resident *data;
    Node *next;
} Node;

typedef enum
{
    OK,
    NOT_MEMORY,
    ERROR_OPENING_FILE,
    NOT_VALID_DATA,
    INPUT_ERROR,
    NO_SEARCH
} Errors;

int isValid(Resident *res)
{
    if (res->birthday.day <= 31 && res->birthday.day > 0 && res->birthday.moth <= 12 && res->birthday.moth > 0 && res->birthday.year > 0 && (res->gender == MALE || res->gender == FEMALE))
        return 1;
    return 0;
}

int read_file(Resident **res, char *file_name)
{

    int size = 10;
    int index = 0;
    int res_count = 0;
    Resident *tmp = NULL;
    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        return ERROR_OPENING_FILE;
    }

    *res = (Resident *)malloc(sizeof(Resident) * size);
    if (res == NULL)
    {
        return NOT_MEMORY;
    }

    while (!feof(file))
    {
        if (index >= size)
        {
            size << 1;
            tmp = (Resident *)realloc(*res, sizeof(Resident) * size);
            if (tmp == NULL)
            {
                free(res);
                return NOT_MEMORY;
            }
            *res = tmp;
        }
        if (fscanf(file, "%s %s %s %d %d %d %d %f", (*res)[index].surname, (*res)[index].name, (*res)[index].patronymic, &(*res)[index].birthday.day, &(*res)[index].birthday.moth, &(*res)[index].birthday.year, &(*res)[index].gender, &(*res)[index].avg_monthy) == 8)
        {
            if (isValid(*res + index) == 0)
                return NOT_VALID_DATA;
            index++;
            res_count = index;
        }
        else
        {
            return NOT_VALID_DATA;
        }
    }

    fclose(file);
    return res_count;
}

int compare_birthday(const void *a, const void *b)
{
    Resident *left = (Resident *)a;
    Resident *right = (Resident *)b;
    return left->birthday.year * 1000 + left->birthday.moth * 100 + left->birthday.day >
           right->birthday.year * 1000 + right->birthday.moth * 100 + right->birthday.day;
}

void push(Node **plist, Resident *res)
{
    Node *p = malloc(sizeof(Node));
    if (p == NULL)
        return;
    p->data = res;
    p->next = *plist;
    *plist = p;
}

Node *create_list(Resident *res, int res_count)
{

    Node *list = NULL;
    for (int i = 0; i < res_count; i++)
    {
        push(&list, &res[i]);
        if (list == NULL)
        {
            return NULL;
        }
    }
    return (list);
}

void print_resident(FILE *stream, Resident *resident)
{
    fprintf(stream, "Surname: %s\n", resident->surname);
    fprintf(stream, "Name: %s\n", resident->name);
    fprintf(stream, "Patronymic: %s\n", resident->patronymic);
    fprintf(stream, "Gender: %s\n", sex[resident->gender]);
    fprintf(stream, "Birth year: %02d.%02d.%02d\n", resident->birthday.day, resident->birthday.moth, resident->birthday.year);
    fprintf(stream, "Average monthly income: %f\n", resident->avg_monthy);
    fputs("-------------------------------\n", stream);
}

void print_residents_array(FILE *stream, Resident *residents, size_t residents_count)
{
    int i = 0;
    for (; i < residents_count; i++)
    {
        print_resident(stream, residents + i);
    }
}

void print_residents_list(FILE *stream, Node *list)
{
    for (Node *p = list; p != NULL; p = p->next)
    {
        print_resident(stream, p->data);
    }
}

int delete_resident(Node **head, int year)
{
    Node *current = *head;
    Node *previous = NULL;

    while (current != NULL)
    {
        if (current->data->birthday.year == year)
        {
            if (previous == NULL)
            {
                // Deleting the first element
                *head = current->next;
            }
            else
            {
                // Deleting an element in the middle or end of the list
                previous->next = current->next;
            }
            free(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }
    return 0;
}

int search_resident_by_years(Resident **res, Node *list, int year, int res_count)
{
    *res = (Resident *)malloc(sizeof(Resident) * res_count);
    int index = 0;
    if (res == NULL)
        return NOT_MEMORY;

    for (Node *p = list; p != NULL; p = p->next)
    {
        if (p->data->birthday.year == year)
        {
            (*res)[index] = *p->data;
            index++;
        }
    }
    return index;
}

void save_to_file(FILE *f_out, Node *list, char *filename)
{
    f_out = fopen(filename, "w");
    if (f_out == NULL)
        return;
    print_residents_list(f_out, list);
    fclose(f_out);
}

void kill_list(Node *list)
{
    for (Node *p = list; p != NULL; p = p->next)
    {
        free(p->data);
    }
    free(list);
}

int main(int argc, char *argv[])
{

    Resident *residents;
    Resident *search_colec;
    Resident *new_resident;
    Node *list;
    unsigned int year;
    unsigned int search_count;
    int residents_count = read_file(&residents, argv[1]);
    switch (residents_count)
    {
    case ERROR_OPENING_FILE:
        printf("Errpr opening file");
        return ERROR_OPENING_FILE;
    case NOT_MEMORY:
        printf("Out of memory");
        return NOT_MEMORY;
    case NOT_VALID_DATA:
        printf("Not valid data");
        return NOT_VALID_DATA;
    default:
        break;
    }

    qsort(residents, residents_count, sizeof(Resident), compare_birthday);
    list = create_list(residents, residents_count);
    if (list == NULL)
    {
        printf("Out of memory");
        free(residents);
        return NOT_MEMORY;
    }

    print_residents_list(stdout, list);

    int menu_id = 0;

    puts("----------Menu----------");
    printf("1. Search\n");
    printf("2. Add\n");
    printf("3. Delate\n");
    printf("4. Continue\n");
    printf("Enter: ");
    if (scanf("%d", &menu_id) != 1)
    {
        printf("Input error");
        return INPUT_ERROR;
    }
    switch (menu_id)
    {
    case 1:
        if (scanf("%d", &year) != 1)
        {
            printf("Input error");
            kill_list(list);
            free(residents);
            return INPUT_ERROR;
        }
        search_count = search_resident_by_years(&search_colec, list, year, residents_count);
        if (search_colec == NULL)
        {
            printf("Out of memory");
            kill_list(list);
            free(residents);
            return NOT_MEMORY;
        }
        else if (search_count == 0)
        {
            printf("No search");
        }
        else
        {
            print_residents_array(stdout, search_colec, search_count);
            free(search_colec);
        }

        break;

    case 2:
        new_resident = (Resident *)malloc(sizeof(Resident));
        printf("Enter the new resident's surname: ");
        scanf("%s", new_resident->surname);
        printf("Enter the new resident's name: ");
        scanf("%s", new_resident->name);
        printf("Enter the new resident's patronymic: ");
        scanf("%s", new_resident->patronymic);
        printf("Enter the new resident's birth year: ");
        if (scanf("%d %d %d", &new_resident->birthday.day, &new_resident->birthday.moth, &new_resident->birthday.year) != 3)
        {
            printf("Input error");
            return INPUT_ERROR;
        }
        printf("Enter the new resident's gender: ");
        if (scanf("%d", &new_resident->gender) != 1)
        {
            printf("Input error");
            return INPUT_ERROR;
        }
        printf("Enter the new resident's average monthly income: ");
        scanf("%f", &new_resident->avg_monthy);
        if (!isValid(new_resident))
        {
            printf("Not valid data");
            break;
        }
        push(&list, new_resident);
        if (list == NULL)
        {
            printf("Out of memory");
            kill_list(list);
            free(residents);
            return NOT_MEMORY;
        }
        printf("New resident added to the list:\n");
        print_resident(stdout, new_resident);
        printf("New list: \n");
        print_residents_list(stdout, list);
        break;
    case 3:
        printf("Enter year: ");
        scanf("%d", &year);
        delete_resident(&list, year);
        print_residents_list(stdout, list);
        break;
    case 4:
        break;
    }

    FILE *f_out;
    save_to_file(f_out, list, argv[2]);
    kill_list(list);
    free(residents);

    return OK;
}
