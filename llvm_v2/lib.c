#include <stdio.h>
#include <stdint.h>

extern void writeInteger(int64_t x) {
    printf("%ld", x);
}

extern void writeChar(int64_t c) {
    printf("%c", (char)c);
}

void writeString(int64_t *str) {
    printf("%s", (char *)str);
}