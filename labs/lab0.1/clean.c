#include <stdio.h>
int main() {
    
    FILE *fp;
    char fileName[255];
    char c;
    char prevc;
    char nextc;

    char *example = "\"Quote\" // Comment";

    
    
    FILE *out; 
    out = fopen("clean.c", "w");

    
    printf("File name: ");
    scanf("%s", fileName);

    
    fp = fopen(fileName, "r+");

    
    if (fp == NULL) {
        printf("File not found\n");
        return 0;
    }

    int p;
    
    while ( (c = getc(fp)) != EOF ) {
        
        if (c == '"') {
            do {
                putc(c, out);
                if (c == '\\') {
                    c = getc(fp);
                    putc(c, out);
                }
            } while((c = getc(fp)) != '"');
            putc(c, out);
        } else if (c == '\'') {
            do {
                putc(c, out);
                if (c == '\\') {
                    c = getc(fp);
                    putc(c, out);
                }
            } while((c = getc(fp)) != '\'');
            putc(c, out);
        } 
        else if (c == '/') {
            prevc = c;
            c = getc(fp);
            
            if (c == '/') {
                while ((c = getc(fp)) != '\n');
                putc('\n', out);
            }
            
            else if (c == '*') {
                prevc = getc(fp);
                c = getc(fp);
                
                while (((prevc = c) != '*') && ((c = getc(fp)) != '/') ){

                }
                c = getc(fp);
            }
            
            else {
                putc(prevc, out);
            }
        }
        

        
        else {
            
            putc(c, out);
        }
    }

    
    fclose(fp);

    return 0;
}