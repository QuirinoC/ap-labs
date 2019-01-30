#include <stdio.h>


int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Not enough args\n");
        exit(1);
    }
    int size = strlen(argv[1]);
    char *newString = stradd(argv[1], argv[2]);
    int found = strfind(newString, argv[3]);
    printf("Initial Lenght      : %d\n", size);
    printf("New String          : %s\n", newString);
    
    printf("SubString was found : %s\n", found ? "yes" : "no");
    
}