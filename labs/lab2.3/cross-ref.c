#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUF_SIZE 0xFFFF

void lowerCase(char *p) {
    for ( ; *p; ++p) *p = tolower(*p);
}

struct node
{
    char word[100];
    int size;
    int **line_data;
};

char *trim(char *word){
    char *helper = (char *) calloc(BUF_SIZE, sizeof(char));
    int i = 0;
    while (*word != '\0') {
        if (*word == ' ') {word++; continue;}
        *(helper + i++) = *word;
        word++;
    }
    return helper;
}

int isNoise(char *word){
    int f = 0;
    char noise_words[10][BUF_SIZE] = {"the","and","to","of","a",};
    for (int i = 0; i < sizeof(noise_words) / sizeof(noise_words[0]) ; i++) {
        if (strcmp(word, noise_words[i]) == 0) {
            f = 1;
            break;
        }
    }
    return f;
}

void increment_size(struct node* ptr)
{
    ptr->size++;
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
    int j = 0;


    //Data
    struct node data[BUF_SIZE];
    int uniq_words = 0;
    int found = 0;

    do {
        //Skip tabs commas, dots
        if (*text == '\t' |
            *text == ',' |
            *text == '.' |
            *text == '!' |
            *text == '?' |
            *text == ';' |
            *text == '*' |
            *text == '\''|
            *text == '"') { 
            text++; continue;
        }

        //Word found
        if (*text == ' ' | *text == '\0' | *text == '\n') {
            *(word + i) = '\0';
            strcpy(word, trim(word));
            lowerCase(word);
            if (strcmp(word, "") != 0) {
                if (isNoise(word) == 0) {
                    //Add word
                    for (j = 0; j < uniq_words; j++) {
                        if(strcmp(data[j].word, word) == 0) {
                            *(data[j].line_data + data[j].size) = line_count;
                            found=1;
                            data[j].size++;
                        }
                    }
                    if (found == 0) {
                        struct node tmp;
                        strcpy(tmp.word, word);
                        tmp.size = 1;
                        tmp.line_data = (int **) calloc(BUF_SIZE, sizeof(int *));
                        *tmp.line_data = line_count;
                        data[uniq_words++] = tmp;
                    }

                   found = 0;
                }
            }
            
            i = 0;
        }
        //Assign new char
        if (*text != '\n') {
            *(word + i++) = *text;
        }

        //Increase line counter
        if (*text == '\n') {
            line_count++;
        }
        text++;

    } while (*(text) !='\0');


    struct node tmp;
    for (i = 0; i < uniq_words; i++){
        tmp = data[i];
        printf("%s [%d]: ", tmp.word, tmp.size);
        for (j = 0; j < tmp.size; j++) {
            printf("%d ", tmp.line_data[j]);
        }
        printf("\n");
    }
}