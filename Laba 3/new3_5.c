#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

typedef struct
{
    int id;
    char first_name[256];
    char second_name[256];
    unsigned int course;
    char group[256];
    int evaluations[5];
    // unsigned int size_colec;
} Student;

// typedef enum
// {
//     ID = 1,
//     NAME,
//     SURNAME,
//     GROUP
// } Flags;

// typedef enum
// {
//     ID,
//     NAME,
//     SURNAME,
//     GROUP
// } Compare;

int read_file(Student **studs, FILE *file)
{
    int stud_count;

    fscanf(file, "%d", &stud_count);
    *studs = (Student *)malloc(sizeof(Student) * stud_count);

    for (int i = 0; i < stud_count; i++)
    {
        fscanf(file, "%d %s %s %d %s", &(*studs)[i].id, (*studs)[i].first_name, (*studs)[i].second_name, &(*studs)[i].course, (*studs)[i].group);
        for (int j = 0; j < 5; j++)
        {
            fscanf(file, "%d", &(*studs)[i].evaluations[j]);
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

int sred_ball(int exam[5])
{
    int sum = 0;
    int res = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += exam[i];
    }
    res = sum / 5;
    return res;
}

void print_studs(Student *studs, int stud_count)
{

    int count = 1;
    // int stud_count = sizeof(studs) / sizeof(studs[0]);
    // printf("%d", stud_count);
    for (int i = 0; i < stud_count; i++)
    {
        printf("Student %d\n", count);
        puts("-------------------------");
        printf("Name: %s %s\n", studs[i].first_name, studs[i].second_name);
        printf("Course: %d\n", studs[i].course);
        printf("Group: %s\n", studs[i].group);
        printf("Exam sred: %d\n", sred_ball(studs[i].evaluations));
        puts("-------------------------");
        count++;
    }
}

void global_sort(Student **studs, int flag, int stud_count)
{

    switch (flag)
    {
    case 1: // sort by id
        qsort(*studs, stud_count, sizeof(Student), compare_id);
        break;
    case 2: // sort by name
        qsort(*studs, stud_count, sizeof(Student), compare_first_name);
        break;
    case 3: // sort by second_name
        qsort(*studs, stud_count, sizeof(Student), compare_second_name);
        break;
    case 4: // sort by group
        qsort(*studs, stud_count, sizeof(Student), compare_group);
        break;
    }
}

Student **group_by_course(Student *studs, int stud_count)
{
    int group_size;
    Student **groups = (Student **)malloc(sizeof(Student *) * 4);
    // TODO: проверка на выделение памяти
    for (int i = 0; i < 4; i++)
    {
        groups[i] = (Student *)malloc(sizeof(Student));
        // TODO: проверка на выделение памяти
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

int search_student(Student **res, Student *studs, int stud_count, int flag, char str[])
{
    *res = (Student *)malloc(sizeof(Student) * stud_count);
    // TODO: проверка на выделение памяти
    int res_count = 0;

    for (int i = 0; i < stud_count; i++)
    {
        if (flag == 1 && (studs[i].id == atoi(str)))
        {
            (*res)[res_count] = studs[i];
            res_count++;
        }
        if (flag == 2 && (!strcmp(studs[i].first_name, str)))
        {
            (*res)[res_count] = studs[i];
            res_count++;
        }
        if (flag == 3 && (!strcmp(studs[i].second_name, str)))
        {
            (*res)[res_count] = studs[i];
            res_count++;
        }
        if (flag == 4 && (!strcmp(studs[i].group, str)))
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

void trace_files(Student **studs, int avMarks, char *filename, int len)
{
    FILE *file, *file1, *file2, *file3, *file4;
    int filename_len = strlen(filename) + 3;
    char *trace_filename = (char *)malloc(sizeof(char) * filename_len);
    // TODO: проверка на выделение памяти
    strcpy(trace_filename, filename);
    trace_filename[filename_len - 1] = '\0';
    trace_filename[filename_len - 2] = '1';
    trace_filename[filename_len - 3] = '_';

    if ((file1 = fopen(trace_filename, "w")) == NULL)
    {
        free(trace_filename);
    }

    trace_filename[filename_len - 2] = '2';

    if ((file2 = fopen(trace_filename, "w")) == NULL)
    {
        fclose(file1);
        free(trace_filename);
    }

    trace_filename[filename_len - 2] = '3';
    if ((file3 = fopen(trace_filename, "w")) == NULL)
    {
        printf("%s", trace_filename);
        fclose(file1);
        fclose(file2);
        free(trace_filename);
    }

    trace_filename[filename_len - 2] = '4';

    if ((file4 = fopen(trace_filename, "w")) == NULL)
    {
        fclose(file1);
        fclose(file2);
        fclose(file3);
        free(trace_filename);
    }
    free(trace_filename);
    for (int i = 0; i < len; i++)
    {

        // int stud_avMarks = sred_ball(student->evaluations);
        // if (stud_avMarks < avMarks)
        //     continue;
        Student *student = studs[i];
        printf("%d", student->course);
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

        if (fprintf(file, "%s %s %d %s\n", student->first_name, student->second_name, student->course, student->group) < 0)
        {
            fclose(file1);
            fclose(file2);
            fclose(file3);
            fclose(file4);
        }
    }
    fclose(file1);
    fclose(file2);
    fclose(file3);
    fclose(file4);
}

int main(int argc, char *argv[])
{
    int sort_id = 0, menu_id = 0, search_id = 0;

    FILE *f_in = fopen(argv[1], "r");
    Student *students;
    // TODO: Проверка на выделение памяти
    int student_count = read_file(&students, f_in);
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
        scanf("%d", &search_id);
        char str[256];
        scanf("%s", str);
        Student *search;
        int search_size = search_student(&search, students, student_count, search_id, str);
        print_studs(search, search_size);
        break;
    case 2:
        printf("1. id\n");
        printf("2. first_name\n");
        printf("3. second_name\n");
        printf("4. group\n");
        printf("Enter sort number: ");
        scanf("%d", &sort_id);
        global_sort(&students, sort_id, student_count);
        print_studs(students, student_count);
        break;
    case 3:
        break;
    }

    Student **groups = group_by_course(students, student_count);
    print_groups(groups);
    int students_av_marks = studs_average_marks(students, student_count);
    printf("%d", students_av_marks);
    trace_files(&students, students_av_marks, argv[1], student_count);

    fclose(f_in);

    return 0;
}
