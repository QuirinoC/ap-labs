#include <stdio.h>
int main() {
    //Input file
    FILE *fp;
    char fileName[255];
    char c;
    char prevc;
    char nextc;

    char *example = "\"Quote\"";

    /*Multi
    line 
    example*/
    
    FILE *out; //Out file
    out = fopen("clean.c", "w");

    //Read file name
    printf("File name: ");
    scanf("%s", fileName);

    //Open file as reading and writing
    fp = fopen(fileName, "r+");

    //File not found
    if (fp == NULL) {
        printf("File not found\n");
        return 0;
    }

    int p;
    //Read char by char until there is no EOF
    while ( (c = getc(fp)) != EOF ) {
        //Comment inside quotes
        if (c == '"') {
        /*
            do {
                putc(c, out);
                prevc = c;
            } while ((c = getc(out)) != '"');
        } else if (c == '\'') {
            do {
                putc(c, out);
                prevc = c;
            } while ((c = getc(out)) != '\'');
        */
        }
        
        else if (c == '/') {
            prevc = c;
            c = getc(fp);
            //Single line comment
            if (c == '/') {
                while ((c = getc(fp)) != '\n');
                putc('\n', out);
            }
            //Multi line comment
            else if (c == '*') {
                prevc = getc(fp);
                c = getc(fp);
                //Read everythin in a file
                while (((prevc = c) != '*') && ((c = getc(fp)) != '/') ){

                }
                c = getc(fp);
            }
            //Division - not a comment
            else {
                putc(prevc, out);
            }
        }
        

        //Other cases
        else {
            printf("%c", c);
            putc(c, out);
        }
    }

    //Close file
    fclose(fp);

    return 0;
}