#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void reverse(char* str) {
    char tmp;
    char *start = str;
    char *end = start + strlen(str) - 1;

    while (end > start) {
        tmp = *start;
        *start = *end;
        *end = tmp;
        start++;
        end--;
    }
}

void uppercase(char* str) {
    int len = strlen(str) - 1;
    for (int i = 0; i < len; i+=2) {
        if (str[i] >= 97 && str[i] <= 122) {
            str[i] -= 32;
        }
    }
}

void new_str(char* str, char* res) {
    int len = strlen(str) - 1;
    int n = 0;
    for (int i = 0; i < len; i++) {
        if (isdigit(str[i])) {
            res[n] = str[i];
            n++;
        }
    }

    for (int i = 0; i < len; i++) {
        if (isalpha(str[i])) {
            res[n] = str[i];
            n++;
        }
    }

    for (int i = 0; i < len; i++) {
        if (!isalpha(str[i]) && !isdigit(str[i])) {
            res[n] = str[i];
            n++;
        }
    }
}

void concatenation(char* str1, char* str2, char* res) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    for (int i = 0; i < len1; i++) {
        res[i] = str1[i];
    }
    for (int i = 0; i < len2; i++) {
        res[len1 + i] = str2[i];
    }
    res[len1 + len2] = '\0';
}

char print(char* str) {
    /*int len = strlen(str) - 1;
    for (int i = 0; i < len; i++) {
        return printf("%c", str[i]);
    }*/
    printf("%s", str);
}

int main(int argc, char* argv[]){
    if (argv[2][0] != '-') {
        printf("Input error: wrong flag");
        return 0;
    }
    if(strlen(argv[2])!=2){
        printf("Input error: wrong flag!");
        return 0;
    }
    if(argc < 3){
        printf("Input error: few arguments!");
        return 0;
    }
    else{
        if(argc == 3 && argv[2][1] == 'c'){
            printf("Input error: too few arguments for that flag!");
            return 0;
        }
        if(argc>4){
            printf("Input error: too many arguments!");
            return 0;
        }
    }
    if (argv[2][1] == 'l') {
        int len = strlen(argv[1]);
        printf("%d", len);
    } else if (argv[2][1] == 'r') {
        reverse(argv[1]);
        print(argv[1]);
    } else if (argv[2][1] == 'u') {
        uppercase(argv[1]);
        print(argv[1]);
    } else if (argv[2][1] == 'n') {
        char* result = (char*)malloc(strlen(argv[1]) * sizeof(char));
        new_str(argv[1], result);
        print(result);
        free(result);
    } else if (argv[2][1] == 'c') {
        char* result = (char*)malloc((strlen(argv[1]) + strlen(argv[3]) + 1) * sizeof(char));
        if (result == NULL)
        {
            printf("Memory allocation error!!1!1");
            return -1;
        }
        concatenation(argv[1], argv[3], result);
        print(result);
        free(result);
    } else {
        printf("Input error: wrong flag!");
    }
    return 0;
}
