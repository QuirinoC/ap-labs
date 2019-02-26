#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 0xFFFF
char *lowerCase(char *word) {

}
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Not enough args");
        exit(-1);
    }
    int fd = open(argv[1], O_RDONLY);
    char *c = (char *) calloc(BUF_SIZE, sizeof(char));

    char *text = (char *) calloc(BUF_SIZE, sizeof(char));
    char *textHelper = text;

    while(read(fd, c, 1) > 0){
        *(textHelper++) = *c;
    } 

    int line_count = 1;

    char *word = (char *) calloc(BUF_SIZE, sizeof(char));
    
    int i = 0;

    do {
        //Skip tabs commas, dots
        if (*text == '\t' | *text == ',' | *text == '.') { text++; continue; }

        //Word found
        if (*text == ' ' | *text == '\0' | *text == '\n') {
            *(word + i) = '\0';
            i = 0;
        }

        //Assign new char
        *(word + i++) = *text;

        //Increase line counter
        if (*text == '\n') {
            line_count++;
        }
        text++;

    } while (*(text) !='\0');
}