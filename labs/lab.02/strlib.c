#include <stdlib.h>

int strlen(char *str) {
    int i = 0;
    while (str[i++] != '\0');
    return i;
}
char *stradd(char *origin, char *addition) {
    int l_size = strlen(origin);
    int r_size = strlen(addition);
    char *p = (char *)malloc(l_size+r_size+1);

    for (int i = 0; i < l_size; i++) {
        p[i] = origin[i];
    }
    for (int i = 0; i < r_size; i++) {
        p[i + l_size - 1] = addition[i];
    }
    return p;
}
int strfind(char *origin, char *substr){
    int origin_size = strlen(origin);
    int substr_size = strlen(substr);



    for (int i = 0; i < origin_size; i++) {
        if (origin[i] == substr[0]) {
            for (int j = 0; j < substr_size; j++) {
                if (substr[j] == '\0') {
                    return 1;
                }
                if (origin[i+j] != substr[j]) {
                    break;
                }
            }  
        }
    }
    return 0;
}