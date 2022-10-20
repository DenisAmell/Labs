#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void func_r(char* str, char* res, unsigned long len){
    for(int i=0;i<len;i++){
        res[i]=str[strlen(str)-i-1];
    }
}

void func_u(char* str, unsigned long len){
    for(int i=0;i<len;i+=2){
        if(str[i]>=97 && str[i]<=122) str[i]-= 32;
    }
}

void func_c(char* str1, char* str2, char* res){
    unsigned long len1=strlen(str1);
    unsigned long len2=strlen(str2);
    for(unsigned long i = 0; i<len1; i++) res[i] = str1[i];
    for(unsigned long i = 0; i<len2; i++) res[len1+i] = str2[i];
}

void func_n(char* str, char* res, unsigned long len){
    int n = 0;
    for(unsigned long i = 0; i<len; i++){
        if(isdigit(str[i])){
            res[n] = str[i];
            n++;
        }
    }
    for(unsigned long i = 0; i<len; i++){
        if(isalpha(str[i])){
            res[n] = str[i];
            n++;
        }
    }
    for(unsigned long i = 0; i<len; i++){
        if(!isalpha(str[i]) && !isdigit(str[i])){
            res[n] = str[i];
            n++;
        }
    }
}

void print(char* str, unsigned long len){
    for(int i=0;i<len;i++) printf("%c", str[i]);
}

int main(int argc, char* argv[]){
    if(argv[2][0] != '-'){
        printf("Input error: wrong flag!");
        return 0;
    }
    if(strlen(argv[2])!=2){
        printf("Input error: wrong flag!");
        return 0;
    }
    if(argc<3){
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

    unsigned long len = strlen(argv[1]);
    if(argv[2][1] == 'l'){
        printf("%lu", len);
    }
    else if(argv[2][1] == 'r'){
        char* reversed = (char*)malloc( strlen(argv[1])* sizeof(char));
        func_r(argv[1],reversed, len);
        print(reversed, len);
        free(reversed);
    }
    else if(argv[2][1] == 'u'){
        char* str = argv[1];
        func_u(str, len);
        print(str, len);
    }
    else if(argv[2][1] == 'n'){
        char* str = argv[1];
        char* result = (char*)malloc( len * sizeof(char));
        func_n(str, result, len);
        print(result, len);
        free(result);
    }
    else if(argv[2][1] == 'c'){
        char* str1 = argv[1];
        char* str2 = argv[3];
        char* result = (char*)malloc( (strlen(str1) + strlen(str2)) * sizeof(char));
        func_c(str1, str2, result);
        print(result, strlen(result));
    }
    else printf("Input error: wrong flag!");

    return 0;
}