#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_LEN 100

typedef struct Employee
{
    unsigned int id;
    char name[MAX_LEN];
    char surname[MAX_LEN];
    double salary;
} Employee;

typedef struct Node Node;

struct Node
{
    Employee *key;
    Node *parent;
    Node *child;
    Node *left;
    Node *right;
    int degree;
};

typedef struct Heap
{
    int size;
    Node *root;
} Heap;

typedef enum
{
    OK,
    ERROR_OPENING_FILE,
    NOT_MEMORY,
    INPUT_ERROR,
} Errors;

void swap(Node **a, Node **b)
{
    Node *tmp = *a;
    *a = *b;
    *b = tmp;
}

int isValid_Str(char *str)
{
    int x = 0;
    while (str[x] != '\0')
    {
        if (!isalpha(str[x]))
        {
            return 0;
        }
        x++;
    }
    return 1;
}

int is_valid(unsigned int *ids, int ids_Size, char *name, char *surname, double salary)
{
    unsigned int toCheck = ids[ids_Size];
    for (int i = 0; i < ids_Size; i++)
    {
        if (ids[i] == toCheck)
            return 0;
    }
    if (!isValid_Str(name) || !isValid_Str(surname))
        return 0;
    if (salary < 0)
        return 0;

    return 1;
}

Employee *read_employees(FILE *file, int *cur_size, int *status_code)
{
    if (!file)
    {
        *status_code = ERROR_OPENING_FILE;
        return NULL;
    }
    int size = 5;
    *cur_size = 0;

    Employee *empl_list = (Employee *)malloc(sizeof(Employee) * size);
    Employee *tmp = NULL;

    if (empl_list == NULL)
    {
        *status_code = NOT_MEMORY;
        return NULL;
    }

    unsigned int id = 0, *tmpIds;
    unsigned int *ids = (unsigned int *)malloc(sizeof(unsigned int) * size);
    if (ids == NULL)
    {
        free(empl_list);
        *status_code = NOT_MEMORY;
        return NULL;
    }

    double salary = 0;
    char ch = 0;

    while (!feof(file))
    {
        if (*cur_size >= size - 1)
        {
            size *= 2;
            if (!(tmp = (Employee *)realloc(empl_list, sizeof(Employee) * size)) || !(tmpIds = (unsigned int *)realloc(ids, sizeof(unsigned int) * size)))
            {
                free(empl_list);
                free(ids);
                *status_code = NOT_MEMORY;
                return NULL;
            }
            empl_list = tmp;
            ids = tmpIds;
        }

        if (fscanf(file, "%u %s %s %lf", &id, empl_list[(*cur_size)].name,
                   empl_list[(*cur_size)].surname, &salary) != 4)
        {
            while (ch != '\n')
                ch = fgetc(file);
            continue;
        }

        ids[(*cur_size)] = id;
        if (!is_valid(ids, (*cur_size), empl_list[(*cur_size)].name, empl_list[(*cur_size)].surname, salary))
        {
            continue;
        }
        empl_list[(*cur_size)].id = id;
        empl_list[(*cur_size)++].salary = salary;
    }

    if (!(tmp = (Employee *)realloc(empl_list, sizeof(Employee) * (*cur_size))))
    {
        free(empl_list);
        free(ids);
        *status_code = NOT_MEMORY;
        return NULL;
    }

    empl_list = tmp;
    free(ids);
    *status_code = OK;
    return empl_list;
}

void print_employ(Employee *list, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%u %s %s %lf\n", list[i].id, list[i].name, list[i].surname, list[i].salary);
    }
}

void init_heap(Heap *heap)
{
    heap->root = NULL;
    heap->size = 0;
}

int insert_node(int compare, Heap *heap, Employee *to_insert)
{
    if (compare != 1 && compare != -1)
        return 5;
    if (!heap)
        return 2;

    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node)
        return NOT_MEMORY;

    new_node->key = to_insert;
    new_node->child = NULL;
    new_node->degree = 0;
    new_node->parent = NULL;

    if (heap->size == 0)
    {
        heap->root = new_node;
        heap->root->right = new_node;
        heap->root->left = new_node;
    }
    else
    {
        Node *prev_right = heap->root->right;
        heap->root->right = new_node;
        new_node->left = heap->root;
        new_node->right = prev_right;
        prev_right->left = new_node;
    }
    if (compare == -1 && (new_node->key->salary < heap->root->key->salary) || compare == 1 && (new_node->key->salary > heap->root->key->salary))
    {
        heap->root = new_node;
    }

    heap->size++;
    return OK;
}

void heap_link(Heap *heap, Node *parent_node, Node *child_node)
{
    child_node->left->right = child_node->right;
    child_node->right->left = child_node->left;
    child_node->left = NULL;
    child_node->right = NULL;
    parent_node->degree++;

    if (parent_node->child != NULL)
    {
        child_node->parent = parent_node;
        child_node->right = parent_node->child->right;
        child_node->left = parent_node->child;
        parent_node->child->right->left = child_node;
        parent_node->child->right = child_node;
    }
    else
    {
        parent_node->child = child_node;
        child_node->parent = parent_node;
        child_node->left = child_node;
        child_node->right = child_node;
    }
}

int heap_consolidate(int compare, Heap *heap)
{
    if (compare != 1 && compare != -1)
    {
        return 5;
    }

    int list_size = log2(heap->size) + 1;
    Node **list_of_trees_degree = (Node **)calloc(list_size, sizeof(Node *));
    if (!list_of_trees_degree)
    {
        return NOT_MEMORY;
    }
    for (int i = 0; i < list_size; i++)
    {
        list_of_trees_degree[i] = NULL;
    }
    Node *curr_node = heap->root, *another_node = NULL;
    int curr_degree = curr_node->degree;

    do
    {
        while (list_of_trees_degree[curr_degree] != NULL)
        {
            another_node = list_of_trees_degree[curr_degree];
            if (another_node == curr_node)
                break;

            if ((compare == -1 && (curr_node->key->salary > another_node->key->salary)) ||
                compare == 1 && (curr_node->key->salary < another_node->key->salary))
                swap(&curr_node, &another_node);

            heap_link(heap, curr_node, another_node);
            list_of_trees_degree[curr_degree] = NULL;
            curr_degree++;
        }
        if (((compare == -1 && (curr_node->key->salary < heap->root->key->salary)) ||
             compare == 1 && (curr_node->key->salary > heap->root->key->salary)) &&
            curr_node != heap->root)
        {
            heap->root = curr_node;
        }

        if (another_node != curr_node)
            list_of_trees_degree[curr_degree] = curr_node;

        curr_node = curr_node->right;
        curr_degree = curr_node->degree;
    } while (curr_node != heap->root);

    heap->root = NULL;
    for (int i = 0; i < list_size; i++)
    {
        curr_node = list_of_trees_degree[i];
        if (curr_node != NULL)
        {
            if (heap->root != NULL)
            {
                heap->root = curr_node;
                heap->root->left = curr_node;
                heap->root->right = curr_node;
            }
            else
            {
                curr_node->left = heap->root;
                curr_node->right = heap->root->right;
                heap->root->right->left = curr_node;
                heap->root->right = curr_node;

                if ((compare == -1 && (curr_node->key->salary < heap->root->key->salary)) ||
                    compare == 1 && (curr_node->key->salary > heap->root->key->salary))
                {
                    heap->root = curr_node;
                }
            }
        }
    }
    free(list_of_trees_degree);
    return OK;
}

int extract_root_node(int compare, Heap *heap, Node **extracted)
{
    if (compare != 1 && compare != -1)
    {
        return 5;
    }
    Node *root = heap->root;
    if (root != NULL)
    {
        if (root->child != NULL)
        {
            Node *curr_node = root->child;
            Node *nextNode = NULL;
            do
            {
                nextNode = curr_node->right;
                curr_node->parent = NULL;
                curr_node->left = root;
                curr_node->right = root->right;
                root->right->left = curr_node;
                root->right = curr_node;
                curr_node = nextNode;
            } while (curr_node != root->child);
        }
        root->left->right = root->right;
        root->right->left = root->left;

        if (root->right == root)
        {
            heap->root = NULL;
        }
        else
        {
            int status_code;
            heap->root = root->right;
            status_code = heap_consolidate(compare, heap);
            if (status_code != OK)
                return status_code;
        }
        heap->size--;
    }
    (*extracted) = root;
    return OK;
}

void print_empl(FILE *stream, Employee to_print)
{
    fprintf(stream, "%u %s %s %lf\n", to_print.id, to_print.name, to_print.surname, to_print.salary);
}

int heap_sort(int compare, Employee *list, int arr_size, FILE *fi)
{
    if (compare != 1 && compare != -1)
        return 5;

    Heap heap;
    init_heap(&heap);

    int code;

    for (int i = 0; i < arr_size; i++)
    {
        code = insert_node(compare, &heap, &(list[i]));
        if (code != OK)
        {
            return code;
        }
    }

    Node *extracted = NULL;

    for (int i = 0; i < arr_size; i++)
    {
        code = extract_root_node(compare, &heap, &extracted);
        if (code != OK)
            return code;

        print_empl(fi, *(extracted->key));
        free(extracted);
        extracted = NULL;
    }
    return OK;
}

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        printf("Input error");
        return INPUT_ERROR;
    }

    FILE *f_in, *f_out;
    if (!(f_in = fopen(argv[1], "r")) || !(f_out = fopen(argv[3], "w")))
    {
        printf("Error opening file");
        return ERROR_OPENING_FILE;
    }

    int status_code;
    int count = 0;

    Employee *employee_list = read_employees(f_in, &count, &status_code);
    switch (status_code)
    {
    case ERROR_OPENING_FILE:
        printf("Error opening file");
        fclose(f_in);
        fclose(f_out);
        return ERROR_OPENING_FILE;
    case NOT_MEMORY:
        printf("Out of memory");
        return NOT_MEMORY;
    default:
        break;
    }

    print_employ(employee_list, count);

    if (!strcmp(argv[2], "-a") || !strcmp(argv[2], "/a"))
    {
        status_code = heap_sort(1, employee_list, count, f_out);
        if (status_code != OK)
        {
            free(employee_list);
            fclose(f_in);
            fclose(f_out);
            return status_code;
        }
    }
    else if (!strcmp(argv[2], "-d") || !strcmp(argv[2], "/d"))
    {
        status_code = heap_sort(-1, employee_list, count, f_out);
        if (status_code != OK)
        {
            free(employee_list);
            fclose(f_in);
            fclose(f_out);
            return status_code;
        }
    }
    else
    {
        free(employee_list);
        fclose(f_in);
        fclose(f_out);
        printf("Input error");
        return INPUT_ERROR;
    }
    free(employee_list);
    fclose(f_in);
    fclose(f_out);

    return OK;
}