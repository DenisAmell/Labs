#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <conio.h>

typedef struct
{
    unsigned int id;
    char first_name[256];
    char second_name[256];
    unsigned int course;
    char group[256];
    int evaluations[5];
} Student;

typedef enum
{
    NOT_MEMORY,
    ERROR_OPENING_FILE,
    ERROR_INPUT,
    STRUCT_NULL,
    NO_VALID_DATA,
    ERROR_WRITE_TO_FILE,
    ERROR_SEARCH,
    OK
} Errors;

int isNum(char c)
{
    int num = 0;
    if ('0' <= c && c <= '9')
    {
        num = c - '0';
        return num;
    }
    return -1;
}

int read_file(Student **studs, FILE *file)
{
    int size = 10;
    int stud_count = 0;
    int i = 0;
    char str_marks[10];
    Student *tmp = NULL;
    *studs = (Student *)malloc(sizeof(Student) * size);
    if (studs == NULL)
    {
        return NOT_MEMORY;
    }

    while (!feof(file))
    {
        if (i >= size)
        {
            size *= 2;
            tmp = (Student *)realloc(*studs, sizeof(Student) * size);
            if (tmp == NULL)
            {
                free(studs);
                return NOT_MEMORY;
            }
            *studs = tmp;
        }
        if (fscanf(file, "%d %s %s %d %s %s", &(*studs)[i].id, (*studs)[i].first_name, (*studs)[i].second_name, &(*studs)[i].course, (*studs)[i].group, str_marks) == 6)
        {
            if (strlen(str_marks) > 5)
                return NO_VALID_DATA;
            for (int j = 0; j < 5; j++)
            {
                if (isNum(str_marks[j]) == -1 || isNum(str_marks[j]) < 2 || isNum(str_marks[j]) > 5)
                    return NO_VALID_DATA;
                (*studs)[i].evaluations[j] = isNum(str_marks[j]);
                // printf("%d ", isNum(str_marks[j]));
            }
            // printf("\n");
            i++;
            stud_count = i;
        }
        else
        {
            return NO_VALID_DATA;
        }
    }

    return stud_count;
}

int compare_id(const void *a, const void *b)
{
    return ((Student *)a)->id - ((Student *)b)->id;
}
int compare_first_name(const void *a, const void *b)
{
    return strcmp(((Student *)a)->first_name, ((Student *)b)->first_name);
}
int compare_second_name(const void *a, const void *b)
{
    return strcmp(((Student *)a)->second_name, ((Student *)b)->second_name);
}
int compare_group(const void *a, const void *b)
{
    return strcmp(((Student *)a)->group, ((Student *)b)->group);
}

int averageMark(int *exam)
{
    int sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += exam[i];
    }
    return ((int)round(sum / 5.0));
}

void print_studs(Student *studs, int stud_count)
{

    int count = 1;
    for (int i = 0; i < stud_count; i++)
    {
        printf("Student %d\n", count);
        puts("-------------------------");
        printf("Name: %s %s\n", studs[i].first_name, studs[i].second_name);
        printf("Course: %d\n", studs[i].course);
        printf("Group: %s\n", studs[i].group);
        printf("Exam sred: %d\n", averageMark(studs[i].evaluations));
        puts("-------------------------");
        count++;
    }
}

int (*compareTo[4])(const void *, const void *) = {compare_id, compare_first_name, compare_second_name, compare_group};

void sort_byTo(Student **studs, int flag, int stud_count)
{
    qsort(*studs, stud_count, sizeof(Student), compareTo[flag]);
}

void free_groups(Student **group, int len)
{
    for (int i = 0; i < len; i++)
    {
        free(group[i]);
    }
    free(group);
}

Student **group_by_course(Student *studs, int stud_count)
{
    int group_size;
    Student **groups = (Student **)malloc(sizeof(Student *) * 4);
    if (groups == NULL)
    {
        free(groups);
        return NULL;
    }
    for (int i = 0; i < 4; i++)
    {
        groups[i] = (Student *)malloc(sizeof(Student));
        if (groups[i] == NULL)
        {
            free_groups(groups, i);
            return NULL;
        }
        group_size = 0;
        for (int j = 0; j < stud_count; j++)
        {
            if (studs[j].course == i + 1)
            {
                groups[i][group_size++] = studs[j];
            }
        }
    }
    return groups;
}

void print_student_group(Student s)
{
    printf("%s %s, Course: %d, Group: %s\n", s.first_name, s.second_name, s.course, s.group);
}

void print_groups(Student **groups)
{
    for (int i = 0; i < 4; i++)
    {
        printf("Course: %d\n", i + 1);
        for (int j = 0; groups[i][j].id != 0; j++)
        {
            if (groups[i][j].course == 0)
            {
                printf("No students\n");
                continue;
            }
            print_student_group(groups[i][j]);
        }
    }
}

int search_byId(Student *studs, char str[])
{
    if (studs->id == atoi(str))
        return 1;
    return 0;
}

int search_byName(Student *studs, char str[])
{
    if (!strcmp(studs->first_name, str))
        return 1;
    return 0;
}

int search_bySurname(Student *studs, char str[])
{
    if (!strcmp(studs->second_name, str))
        return 1;
    return 0;
}

int search_byGroup(Student *studs, char str[])
{
    if (!strcmp(studs->group, str))
        return 1;
    return 0;
}

int (*search_byTo[4])(Student *, char[]) = {search_byId, search_byName, search_bySurname, search_byGroup};

int search_student(Student **res, Student *studs, int stud_count, int flag, char str[])
{
    if (flag < 0 || flag > 5)
        return ERROR_INPUT;
    *res = (Student *)malloc(sizeof(Student) * stud_count);
    if (res == NULL)
        return NOT_MEMORY;

    int res_count = 0;

    for (int i = 0; i < stud_count; i++)
    {
        if (search_byTo[flag](&studs[i], str) == 1)
        {
            (*res)[res_count] = studs[i];
            res_count++;
        }
    }
    return res_count;
}

int studs_average_marks(Student *studs, int len)
{
    int sum = 0;
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            sum += studs[i].evaluations[j];
        }
    }
    return ((int)round(sum / (double)(5 * len)));
}

int studentsAverageMark(Student *list, int len)
{
    int sumMarks = 0;
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < 5; j++)
            sumMarks += list[i].evaluations[j];
    }
    return ((int)round(sumMarks / (double)(5 * len)));
}

int trace_Files(char *origFilename, Student *studs, int len)
{
    if (studs == NULL)
        return STRUCT_NULL;
    FILE *file1, *file2, *file3, *file4;
    int lenFilename = strlen(origFilename) + 3;
    char *filename = (char *)malloc(sizeof(char) * lenFilename);
    if (filename == NULL)
        return NOT_MEMORY;
    strcpy(filename, origFilename);
    filename[lenFilename - 1] = '\0';
    filename[lenFilename - 2] = '1';
    filename[lenFilename - 3] = '_';
    if ((file1 = fopen(filename, "w")) == NULL)
    {
        free(filename);
        return ERROR_OPENING_FILE;
    }
    filename[lenFilename - 2] = '2';
    if ((file2 = fopen(filename, "w")) == NULL)
    {
        fclose(file1);
        free(filename);
        return ERROR_OPENING_FILE;
    }
    filename[lenFilename - 2] = '3';
    if ((file3 = fopen(filename, "w")) == NULL)
    {
        fclose(file1);
        fclose(file2);
        free(filename);
        return ERROR_OPENING_FILE;
    }
    filename[lenFilename - 2] = '4';
    if ((file4 = fopen(filename, "w")) == NULL)
    {
        fclose(file1);
        fclose(file2);
        fclose(file3);
        free(filename);
        return ERROR_OPENING_FILE;
    }
    free(filename);

    int avMark = studentsAverageMark(studs, len);
    FILE *file;

    for (int i = 0; i < len; i++)
    {
        Student *student = &studs[i];
        int studentAvMark = averageMark(student->evaluations);
        if (studentAvMark <= avMark)
            continue;
        switch (student->course)
        {
        case 1:
            file = file1;
            break;
        case 2:
            file = file2;
            break;
        case 3:
            file = file3;
            break;
        case 4:
            file = file4;
            break;
        }
        if (fprintf(file, "%s\t%s\t%d\t%s\t%d\n", student->first_name,
                    student->second_name, student->course, student->group,
                    studentAvMark) < 0)
        {
            fclose(file1);
            fclose(file2);
            fclose(file3);
            fclose(file4);
            return ERROR_WRITE_TO_FILE;
        }
    }
    fclose(file1);
    fclose(file2);
    fclose(file3);
    fclose(file4);
    return OK;
}

int main(int argc, char *argv[])
{
    int sort_id = 0, menu_id = 0, search_id = 0, student_count;
    FILE *f_in = fopen(argv[1], "r");
    Student *students;
    int status_code = read_file(&students, f_in);

    switch (status_code)
    {
    case NOT_MEMORY:
        printf("Out of memory");
        return NOT_MEMORY;

    case NO_VALID_DATA:
        printf("No valid data");
        return NO_VALID_DATA;

    case ERROR_OPENING_FILE:
        printf("Error opening file");
        return ERROR_OPENING_FILE;
    default:
        student_count = status_code;
        break;
    }

    // printf("%d", student_count);
    print_studs(students, student_count);
    puts("-----------Menu-----------");
    printf("1. Search\n");
    printf("2. Sort\n");
    printf("3. Continue\n");
    puts("--------------------------");
    printf("Enter: ");
    scanf("%d", &menu_id);

    switch (menu_id)
    {
    case 1:
        printf("1. id\n");
        printf("2. first_name\n");
        printf("3. second_name\n");
        printf("4. group\n");
        printf("Enter search number: ");
        if (scanf("%d", &search_id) != 1)
        {
            printf("Input error");
            fclose(f_in);
            return ERROR_INPUT;
        }

        char str[256];
        printf("Enter str: ");
        scanf("%s", str);
        Student *search;
        int search_size;

        status_code = search_student(&search, students, student_count, search_id - 1, str);
        switch (status_code)
        {
        case NOT_MEMORY:
            printf("Out of memory");
            fclose(f_in);
            return NOT_MEMORY;
        case ERROR_INPUT:
            printf("Input error");
            fclose(f_in);
            return ERROR_INPUT;
        default:
            search_size = status_code;
            break;
        }
        print_studs(search, search_size);
        free(search);
        break;
    case 2:
        printf("1. id\n");
        printf("2. first_name\n");
        printf("3. second_name\n");
        printf("4. group\n");
        printf("Enter sort number: ");
        if (scanf("%d", &sort_id) != 1)
        {
            printf("Input error");
            fclose(f_in);
            return ERROR_INPUT;
        }
        else if (sort_id < 1 && sort_id > 4)
        {
            printf("Input error");
            fclose(f_in);
            return ERROR_INPUT;
        }
        sort_byTo(&students, sort_id - 1, student_count);
        print_studs(students, student_count);
        break;
    case 3:
        break;
    default:
        printf("Input error");
        fclose(f_in);
        return ERROR_INPUT;
    }

    status_code = trace_Files(argv[1], students, student_count);
    if (status_code != OK)
    {
        switch (status_code)
        {
        case ERROR_OPENING_FILE:
            printf("Error opening file");
            break;
        case NOT_MEMORY:
            printf("Out of memory");
            break;
        case ERROR_WRITE_TO_FILE:
            printf("Error write to file");
            break;
        }
        free(students);
        fclose(f_in);
        return ERROR_SEARCH;
    }

    Student **groups = group_by_course(students, student_count);
    if (groups == NULL)
    {
        printf("Out of memory");
        free(students);
        fclose(f_in);
        return NOT_MEMORY;
    }
    print_groups(groups);

    free_groups(groups, 4);
    free(students);
    fclose(f_in);
    return OK;
}
