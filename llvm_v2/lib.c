#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

extern void writeInteger(int32_t x) {
    printf("%d", x);
}

extern void writeChar(int8_t c) {
    printf("%c", c);
}

extern void writeString(int8_t *str) {
    int i = 0;
    while(str[i]!='\0'){
        printf("%c", str[i]);
        i=i+1;
    }
}


extern int32_t readInteger() {
    int32_t x;
    if (scanf("%d" , &x))
        return x;
    else{
        printf("\033[1;31mexecution error:\n\t\033[0m");
        printf("Expected \033[1;35minteger\033[0m as input.\n"); exit(1);
    }
}

extern int8_t readChar() {
    int8_t c;
    if (scanf("%c", &c))
        return c;
    else{
        printf("\033[1;31mexecution error:\n\t\033[0m");
        printf("Expected \033[1;35mcharacter\033[0m as input.\n"); exit(1);
    }
}

extern void readString(int8_t *str) {
    char c;
    int i = 0;
    while(1){
        c = getchar();
        if(c == '\n') break;
        str[i] = c; 
        i=i+1;
    }
    str[i] = '\0';
}

extern int32_t ascii(int8_t x) {
    return (int)x;
}


extern int32_t chr(int32_t x) {
    return (char)x;
}


extern int32_t strlenGrace(int8_t *str) {
    int i = 0;
    while(str[i]!='\0'){
        i=i+1;
    }
    return i;
}


extern int32_t strcmpGrace(int8_t *str1, int8_t *str2) {
    int i = 0;
    int result = 0;
    while(str1[i] !='\0' && str2[i] !='\0'){
        if(str1[i]!=str2[i]){
            result = -1;
            break;
        }
        i=i+1;
    }
    if(str1[i]!=str2[i]) result = -1;
    return result;
}

extern void strcpyGrace(int8_t *str1, int8_t *str2) {
    int i = 0;
    while(str2[i] !='\0'){
        str1[i]=str2[i];
        i=i+1;
    }
    str1[i]=str2[i];
}
extern void strcatGrace(int8_t *str1, int8_t *str2) {
    int i = 0;
    while(str1[i] !='\0'){
        str1[i] = str1[i];
        i=i+1;
    }
    int j = 0;
    while(str2[j] !='\0'){
        str1[i] = str2[j];
        i=i+1;
        j=j+1;
    }
    str1[i] = str2[j];
}